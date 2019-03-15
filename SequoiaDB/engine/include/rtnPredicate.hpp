/*******************************************************************************


   Copyright (C) 2011-2014 SequoiaDB Ltd.

   This program is free software: you can redistribute it and/or modify
   it under the term of the GNU Affero General Public License, version 3,
   as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warrenty of
   MARCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program. If not, see <http://www.gnu.org/license/>.

   Source File Name = rtnPredicate.hpp

   Descriptive Name = RunTime Access Plan Manager Header

   When/how to use: this program may be used on binary and text-formatted
   versions of Runtime component. This file contains structure for runtime
   predicate, which is records the predicates assigned by user.

   Dependencies: N/A

   Restrictions: N/A

   Change Activity:
   defect Date        Who Description
   ====== =========== === ==============================================
          09/14/2012  TW  Initial Draft

   Last Changed =

*******************************************************************************/
#ifndef RTNPREDICATE_HPP__
#define RTNPREDICATE_HPP__

#include "core.hpp"
#include "oss.hpp"
#include "../bson/bson.h"
#include "pd.hpp"
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace bson ;
using namespace std ;
namespace engine
{

   // Each query could have no more than 16 parameters
   #define RTN_MAX_PARAM_NUM                    ( 16 )

   class _rtnParamList : public SDBObject
   {
      protected :
         typedef struct _rtnParam
         {
            _rtnParam ()
            : _doneByPred( FALSE )
            {
            }

            BSONElement _param ;
            BOOLEAN     _doneByPred ;
         } _rtnParam, rtnParam ;

      public :
         _rtnParamList () ;

         virtual ~_rtnParamList () ;

         void toBSON ( BSONObjBuilder &builder ) const ;
         BSONObj toBSON () const ;
         string toString () const ;

         OSS_INLINE BOOLEAN isEmpty () const
         {
            return _paramNum <= 0 ;
         }

         OSS_INLINE void clearParams ()
         {
            _paramNum = 0 ;
         }

         OSS_INLINE INT8 getCurIndex () const
         {
            return _paramNum ;
         }

         OSS_INLINE BOOLEAN canParameterize () const
         {
            return _paramNum < RTN_MAX_PARAM_NUM ;
         }

         INT8 addParam ( const BSONElement &param ) ;

         BSONElement getParam ( INT8 index ) const
         {
            SDB_ASSERT( index >= 0 && index < _paramNum,
                        "index is invalid" ) ;
            return _params[ index ]._param ;
         }

         void setDoneByPred ( INT8 index ) ;

         OSS_INLINE BOOLEAN isDoneByPred ( INT8 index ) const
         {
            SDB_ASSERT( index >= 0 && index < _paramNum,
                        "index is invalid" ) ;
            return _params[ index ]._doneByPred ;
         }

      protected :
         INT8        _paramNum ;
         rtnParam    _params[ RTN_MAX_PARAM_NUM ] ;
   } ;

   typedef class _rtnParamList rtnParamList ;

   INT32 rtnKeyCompare ( const BSONElement &l, const BSONElement &r ) ;

   BSONObj rtnKeyGetMinType ( INT32 bsonType ) ;
   BSONObj rtnKeyGetMaxType ( INT32 bsonType ) ;
   BSONObj rtnKeyGetMinForCmp ( INT32 bsonType, BOOLEAN mixCmp ) ;
   BSONObj rtnKeyGetMaxForCmp ( INT32 bsonType, BOOLEAN mixCmp ) ;

   // bound of a field, say {c1:5}
   // bound doesn't include upper/lower information, it only have the
   // BSONElement indicating the boundary and whether if it's inclusive
   class rtnKeyBoundary : public SDBObject
   {
   public :
      rtnKeyBoundary ()
      {
         _inclusive = FALSE ;
         _parameterized = FALSE ;
      }
      rtnKeyBoundary ( const BSONElement &bound, BOOLEAN inclusive )
      {
         _bound = bound ;
         _inclusive = inclusive ;
         _parameterized = FALSE ;
      }
      BSONElement _bound ;
      BOOLEAN _inclusive ;
      BOOLEAN _parameterized ;
      BOOLEAN operator==( const rtnKeyBoundary &r ) const
      {
         return _inclusive == r._inclusive &&
                _bound.woCompare ( r._bound ) == 0 ;
      }
      void flipInclusive ()
      {
         _inclusive = !_inclusive ;
      }
   } ;
   enum RTN_SSK_VALUE_POS
   {
      RTN_SSK_VALUE_POS_LT = -2,
      RTN_SSK_VALUE_POS_LET,
      RTN_SSK_VALUE_POS_WITHIN,
      RTN_SSK_VALUE_POS_GET,
      RTN_SSK_VALUE_POS_GT,
   } ;

   // return TRUE if the pos is at edge
   OSS_INLINE BOOLEAN rtnSSKPosAtEdge ( RTN_SSK_VALUE_POS pos )
   {
      return pos == RTN_SSK_VALUE_POS_LET ||
             pos == RTN_SSK_VALUE_POS_GET ;
   }

   enum RTN_SSK_RANGE_POS
   {
      RTN_SSK_RANGE_POS_LT = -3,
      RTN_SSK_RANGE_POS_LET,
      RTN_SSK_RANGE_POS_LI,
      RTN_SSK_RANGE_POS_CONTAIN,
      RTN_SSK_RANGE_POS_RI,
      RTN_SSK_RANGE_POS_RET,
      RTN_SSK_RANGE_POS_RT
   } ;

   OSS_INLINE BOOLEAN rtnSSKRangeAtEdge ( RTN_SSK_RANGE_POS pos )
   {
      return pos == RTN_SSK_RANGE_POS_LET ||
             pos == RTN_SSK_RANGE_POS_RET ;
   }

   // bson::MinKey - 1
   #define RTN_KEY_MAJOR_DEFAULT ( -2 )

   // range of a start/stop key
   class rtnStartStopKey : public SDBObject
   {
   public :
      rtnKeyBoundary _startKey ;
      rtnKeyBoundary _stopKey ;

      // equality = 1 means this is == key, otherwise it's ranged query key
      mutable INT8 _equality ;

      INT32 _majorType ;

      BOOLEAN isValid () const
      {
         INT32 result = _startKey._bound.woCompare ( _stopKey._bound, FALSE);
         return ( result<0 || (result == 0 && _startKey._inclusive &&
                               _stopKey._inclusive) ) ;
      }
      BOOLEAN isEquality () const
      {
         if ( -1 == _equality )
         {
            _equality = (_startKey._inclusive &&
                         _startKey == _stopKey)?
                        1:0 ;
         }
         return 1 == _equality ;
      }
      rtnStartStopKey()
      {
         _equality = -1 ;
         _majorType = RTN_KEY_MAJOR_DEFAULT ;
      }
      rtnStartStopKey ( const BSONElement &e )
      {
         _startKey._bound = _stopKey._bound = e ;
         _startKey._inclusive = _stopKey._inclusive = TRUE ;
         _equality = 1 ;
         _majorType = e.type() ;
      }
      rtnStartStopKey( INT32 imPossibleCondition )
      {
         _startKey._bound = _stopKey._bound = maxKey.firstElement() ;
         _startKey._inclusive = _stopKey._inclusive = FALSE ;
         _equality = 1 ;
         _majorType = RTN_KEY_MAJOR_DEFAULT ; ;
      }
      string toString() const ;
      // convert rtnStartStopKey to bson element
      // { "a": { <start key BSONElement>, "i": true },
      //   "o": { <stop key BSONElement } }
      // Note if the key is inclusive, we'll have "i", otherwise "i" will not be
      // displayed
      BSONObj toBson () const ;
      // convert from BSONObj, return TRUE if success, otherwise return FALSE
      // for bad object
      BOOLEAN fromBson ( BSONObj &ob ) ;
      // reset to min/max key
      void reset () ;
      // RTN_SSK_VALUE_POS_LT: ele is less than startKey
      // RTN_SSK_VALUE_POS_LET: ele is equal to startKey
      // RTN_SSK_VALUE_POS_WITHIN: ele is greater than startKey and less than
      // stopKey
      // RTN_SSK_VALUE_POS_GET: ele is equal to stopKey
      // RTN_SSK_VALUE_POS_GT: ele is greater than stopKey
      RTN_SSK_VALUE_POS compare ( BSONElement &ele, INT32 dir ) const ;

      RTN_SSK_RANGE_POS compare ( rtnStartStopKey &key, INT32 dir ) const ;
   } ;

   typedef vector< rtnStartStopKey > RTN_SSKEY_LIST ;

   // if we want to find the max bound of start keys, we want to return the one
   // list of start/stop key that doesn't intersect with each other on one field
   class rtnPredicate : public SDBObject
   {
   private:
      BOOLEAN _isInitialized ;
      vector<BSONObj> _objData ;

      // _equalFlag == 1 means is equal operation
      INT8 _equalFlag ;

      // _allEqualFlag == 1 means all start-stop key-pairs are equal operation
      INT8 _allEqualFlag ;

      // Evaluate for optimizer
      BOOLEAN _evaluated ;
      BOOLEAN _allRange ;
      double _selectivity ;

      INT8 _paramIndex ;
      INT8 _fuzzyIndex ;

      // this is used when creating new bsonobject in the class
      BSONObj addObj ( const BSONObj &o )
      {
         _objData.push_back(o) ;
         return o ;
      }
      void finishOperation ( const RTN_SSKEY_LIST &newkeys,
                             const rtnPredicate &other ) ;
   public:
      RTN_SSKEY_LIST _startStopKeys ;
      rtnPredicate ( )
      {
         rtnPredicate ( BSONObj().firstElement(), 0, FALSE, TRUE ) ;
      }
      rtnPredicate ( const BSONElement &e, INT32 opType, BOOLEAN isNot,
                     BOOLEAN mixCmp,
                     INT8 paramIndex = -1,
                     INT8 fuzzyOptr = -1 ) ;
      ~rtnPredicate ()
      {
         _startStopKeys.clear() ;
      }
      // intersection operation for two keysets
      const rtnPredicate &operator&= (rtnPredicate &right) ;
      // union operation for two keysets
      const rtnPredicate &operator|= (const rtnPredicate &right) ;
      // exclude operation for two keysets
      const rtnPredicate &operator-= (const rtnPredicate &right) ;
      const rtnPredicate &operator= (const rtnPredicate &right) ;
      BOOLEAN operator<= ( const rtnPredicate &r ) const ;
      void reverse ( rtnPredicate &result ) const ;
      BOOLEAN isInit()
      {
         return _isInitialized ;
      }
      // return the start key from the lowest range
      // eoo means invalid
      BSONElement min() const
      {
         if ( !isEmpty() )
         {
            return _startStopKeys[0]._startKey._bound ;
         }
         return BSONElement() ;
      }
      // return the stop key from the largest range
      // eoo means invalid
      BSONElement max() const
      {
         if ( !isEmpty() )
         {
            return _startStopKeys[_startStopKeys.size()-1]._stopKey._bound ;
         }
         return BSONElement() ;
      }
      // return the inclusiveness from the startkey from the lowest range
      BOOLEAN minInclusive() const
      {
         if ( !isEmpty() )
         {
            return _startStopKeys[0]._startKey._inclusive ;
         }
         return FALSE ;
      }
      // return the inclusiveness from the stopkey from the largest range
      BOOLEAN maxInclusive() const
      {
         if ( !isEmpty() )
         {
            return _startStopKeys[_startStopKeys.size()-1]._stopKey._inclusive ;
         }
         return FALSE ;
      }
      BOOLEAN isEquality ()
      {
         if ( -1 == _equalFlag )
         {
            _equalFlag = ( !isEmpty() && min().woCompare(max(), FALSE)== 0 &&
                           maxInclusive() && minInclusive() ) ? 1 : 0 ;
         }
         return _equalFlag == 1 ;
      }
      BOOLEAN isAllEqual ()
      {
         if ( -1 == _allEqualFlag )
         {
            UINT32 equalCount = 0 ;
            for ( RTN_SSKEY_LIST::iterator iterSSKey = _startStopKeys.begin() ;
                  iterSSKey != _startStopKeys.end() ;
                  iterSSKey ++ )
            {
               if ( iterSSKey->isEquality() )
               {
                  equalCount ++ ;
               }
               else
               {
                  break ;
               }
            }
            // All equal operators
            _allEqualFlag = ( equalCount == _startStopKeys.size() ) ? 1 : 0 ;
         }
         return _allEqualFlag == 1 ;
      }
      BOOLEAN isEmpty() const
      {
         return _startStopKeys.empty() ;
      }
      // isGeneric = TRUE means the predicate will match everything in the
      // collection, which means the predicate doesn't do any filtering
      BOOLEAN isGeneric() const
      {
         return !isEmpty() && _startStopKeys.size() == 1 &&
                 _startStopKeys[0]._startKey._inclusive &&
                 _startStopKeys[0]._stopKey._inclusive &&
                 _startStopKeys[0]._startKey._bound == minKey.firstElement() &&
                 _startStopKeys[0]._stopKey._bound == maxKey.firstElement() ;
      }
      string toString() const ;

      OSS_INLINE BOOLEAN isEvaluated () const
      {
         return _evaluated ;
      }

      OSS_INLINE BOOLEAN isAllRange () const
      {
         return _allRange ;
      }

      OSS_INLINE double getSelectivity () const
      {
         return _selectivity ;
      }

      OSS_INLINE void setSelectivity ( double selectivity,
                                       BOOLEAN allRange )
      {
         _evaluated = TRUE ;
         _allRange = allRange ;
         _selectivity = selectivity ;
      }

      BOOLEAN bindParameters ( rtnParamList &parameters,
                               BOOLEAN markDone = TRUE ) ;

   protected :
      // Helper functions for create predicate
      INT32 _initIN ( const BSONElement &e, BOOLEAN isNot,
                      BOOLEAN mixCmp, BOOLEAN expandRegex ) ;
      INT32 _initRegEx ( const BSONElement &e, BOOLEAN isNot ) ;
      INT32 _initET ( const BSONElement &e, BOOLEAN isNot ) ;
      INT32 _initNE ( const BSONElement &e, BOOLEAN isNot ) ;
      INT32 _initLT ( const BSONElement &e, BOOLEAN isNot, BOOLEAN inclusive,
                      BOOLEAN mixCmp ) ;
      INT32 _initGT ( const BSONElement &e, BOOLEAN isNot, BOOLEAN inclusive,
                      BOOLEAN mixCmp ) ;
      INT32 _initALL ( const BSONElement &e, BOOLEAN isNot ) ;
      INT32 _initMOD ( const BSONElement &e, BOOLEAN isNot ) ;
      INT32 _initEXISTS ( const BSONElement &e, BOOLEAN isNot ) ;
      INT32 _initISNULL ( const BSONElement &e, BOOLEAN isNot ) ;

      INT32 _initFullRange () ;
      INT32 _initTypeRange ( BSONType type, BOOLEAN forCmp ) ;
      INT32 _initMinRange ( BOOLEAN startIncluded ) ;
   } ;

   typedef vector< rtnPredicate > RTN_PREDICATE_LIST ;
   typedef vector< RTN_PREDICATE_LIST > RTN_PARAM_PREDICATE_LIST ;
   typedef map< string, rtnPredicate > RTN_PREDICATE_MAP ;
   typedef map< string, RTN_PREDICATE_LIST > RTN_PARAM_PREDICATE_MAP ;

   // This set is created when receiving a query. It contains user search
   // condition predicates from user input for all fields
   class _rtnPredicateSet : public SDBObject
   {
   public:
      const rtnPredicate &predicate ( const CHAR *fieldName ) const ;
      const RTN_PREDICATE_LIST &paramPredicate ( const CHAR *fieldName ) const ;
      const RTN_PREDICATE_MAP &predicates() const { return _predicates ; }
      RTN_PREDICATE_MAP &predicates() { return _predicates ; }
      INT32 matchLevelForIndex ( const BSONObj &keyPattern ) const ;
      INT32 addPredicate ( const CHAR *fieldName, const BSONElement &e,
                           INT32 opType, BOOLEAN isNot, BOOLEAN mixCmp,
                           BOOLEAN addToParam, INT8 paramIndex,
                           INT8 fuzzyIndex ) ;
      UINT32 getSize () const { return _predicates.size() ; }
      void clear()
      {
         _predicates.clear() ;
         _paramPredicates.clear() ;
      }
      string toString() const ;

      BSONObj toBson() const ;

   private:
      RTN_PREDICATE_MAP _predicates ;
      RTN_PARAM_PREDICATE_MAP _paramPredicates ;
   } ;
   typedef class _rtnPredicateSet rtnPredicateSet ;

   class _ixmIndexCB ;
   class _rtnPredicateListIterator ;
   // This list is created when optimizer confirming to use a specific index, it
   // will be created by using rtnPredicateSet ( which defined by the input
   // query ) and ixmIndexCB ( which defined by the index ). This List will be
   // created using the input query based on the index definition, including a
   // list by the order of index def, which represents the matching condition
   // for the given index
   // each index key on disk will be sent to matchesKey function to match
   class _rtnPredicateList : public SDBObject
   {
   public :
         _rtnPredicateList () ;

         virtual ~_rtnPredicateList () ;

         void clear () ;

         // Build the fixed predicate list
         INT32 initialize ( const rtnPredicateSet &predSet,
                            const BSONObj &keyPattern,
                            INT32 direction,
                            UINT32 &addedLevel ) ;

         // Build the predicate list with parameters, and generate
         // parameterized predicate list to bind parameters quickly for
         // similar queries in the future
         INT32 initialize ( const rtnPredicateSet &predSet,
                            const BSONObj &keyPattern,
                            INT32 direction,
                            rtnParamList &parameters,
                            RTN_PARAM_PREDICATE_LIST &paramPredList ) ;

         // Build the predicate list from parameterized predicate list with
         // current parameters
         INT32 initialize ( const RTN_PARAM_PREDICATE_LIST &paramPredList,
                            const BSONObj &keyPattern,
                            INT32 direction,
                            rtnParamList &parameters ) ;

         UINT32 size() { return _predicates.size(); }
         BSONObj startKey() const ;
         BSONObj endKey() const ;
         BSONObj obj() const ;
         BOOLEAN matchesKey ( const BSONObj &key ) const ;
         string toString() const ;
         BSONObj getBound() const ;

         OSS_INLINE INT32 getDirection() const
         {
            return _direction ;
         }

         OSS_INLINE void setDirection ( INT32 dir )
         {
            _direction = dir ;
         }

         OSS_INLINE BOOLEAN isInitialized () const
         {
            return _initialized ;
         }

         OSS_INLINE BOOLEAN isFixedPredList () const
         {
            return _fixedPredList ;
         }

      protected :
         void _addPredicate ( const BSONElement &e,
                              INT32 direction,
                              const rtnPredicate &pred ) ;

         void _addEmptyPredicate () ;

         INT32 matchingLowElement ( const BSONElement &e, INT32 i,
                                    BOOLEAN direction,
                                    BOOLEAN &lowEquality ) const ;
         BOOLEAN matchesElement ( const BSONElement &e, INT32 i,
                                  BOOLEAN direction ) const ;

      protected :
         BOOLEAN _initialized ;
         BOOLEAN _fixedPredList ;
         RTN_PREDICATE_LIST _predicates ;
         INT32 _direction ;
         BSONObj _keyPattern ;

         friend class _rtnPredicateListIterator ;
   } ;
   typedef class _rtnPredicateList rtnPredicateList ;

   enum rtnPredicateCompareResult
   {
      MATCH = 0,
      LESS,
      GREATER
   } ;
   // this class is used to iterate an ordered predicate list
   class _rtnPredicateListIterator : public SDBObject
   {
   private :
      const rtnPredicateList &_predList ;
      vector <const BSONElement *>_cmp ;
      vector <BOOLEAN> _inc ;
      vector <INT32>   _currentKey ;
      vector <INT32>   _prevKey ;
      // this variable is passed to ixm. When this variable is TRUE, it means we
      // are going to jump over the current key
      BOOLEAN _after ;
   public :
      _rtnPredicateListIterator ( const rtnPredicateList &predList ) ;
      INT32 advance ( const BSONObj &curr ) ;
      const vector<const BSONElement *> &cmp() const { return _cmp ; }
      const vector<BOOLEAN> &inc() const { return _inc ; }
      void reset() ;
      BOOLEAN after() { return _after ; }
   private :
      rtnPredicateCompareResult
            validateCurrentStartStopKey ( INT32 keyIdx,
                                          const BSONElement &currElt,
                                          BOOLEAN reverse,
                                          BOOLEAN &hitUpperInclusive ) ;
      INT32 advanceToLowerBound ( INT32 i ) ;
      INT32 advancePast ( INT32 i ) ;
      INT32 advancePastZeroed ( INT32 i ) ;
   } ;
   typedef class _rtnPredicateListIterator rtnPredicateListIterator ;
}

#endif
