<?php
/*******************************************************************************
   Copyright (C) 2012-2018 SequoiaDB Ltd.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*******************************************************************************/

/** \file class_cs.php
    \brief collection space class
 */

/**
 * SequoiaCS Class. To get this Class object must be call SequoiaDB::selectCS or SequoiaDB::getCS.
 */
class SequoiaCS
{
   /**
    * Drop collection space.
    *
    * @return Returns the result, default return array.
    *
    * @retval array   array( 'errno' => 0 )
    * @retval string  { "errno": 0 }
    *
    * Example:
    * @code
    * $err = $cs -> drop() ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to drop collection space, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function drop(){}

   /**
    * Get the specified collection space name.
    *
    * @return Returns the collection space name.
    *
    * @retval string &lt;cs_name&gt;
    *
    * Example:
    * @code
    * $csName = $cs -> getName() ;
    * $err = $db -> getLastErrorMsg() ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to get collection space name, error code: ".$err['errno'] ;
    *    return ;
    * }
    * echo "Collection space name is: ".$csName ;
    * @endcode
   */
   public function getName(){}
   
   /**
    * Get the specified collection, if is not exist, will auto create.
    *
    * @param $name	the string argument. The collection name.
    *
    * @param $options an array or the string argument. When the collection is created, $options into force.
    *                                                  @code
    *                                                  ShardingKey         : The partition key.
    *                                                  ShardingType        : The partition type.
    *                                                  Partition           : Number of partitions, ShardingType is 'hash', represented the number of hash partitions, its value must be a power of 2, range [ 2^3, 2^20 ], default 1024.
    *                                                  ReplSize            : Copy write by default number is 1.
    *                                                  Compressed          : Data compression, default false.
    *                                                  CompressionType     : Types of compression, default 'snappy'.
    *                                                  IsMainCL            : Main partition, default false. 
    *                                                  AutoSplit           : Automatic split, defualt true.
    *                                                  Group               : To create a replication group.
    *                                                  AutoIndexId         : Collection is automatically created using the _id field is called '$id' a unique index, default true.
    *                                                  EnsureShardingIndex : Collection is automatically created using the ShardingKey contains the field names for the '$shard' index, default true.
    *                                                  AutoIncrement       : Auto increment.
    *                                                  @endcode
    *
    * @return Returns a new SequoiaCL object.
    *
    * @retval SequoiaCL Object
    *
    * Example:
    * @code
    * $cl = $cs -> selectCL( 'bar', array( 'Compressed' => true ) ) ;
    * if( empty( $cl ) ) {
    *    $err = $db -> getLastErrorMsg() ;
    *    echo "Failed to call selectCL, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
    *
    * Example: create auto increment collection
    * @code
    * $err = $cs -> selectCL( 'bar', array( 'AutoIncrement' => array( 'Field' => 'a', 'MaxValue' => 20000 ) ) ) ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to call selectCL, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function selectCL( string $name, array|string $options = null ){}

   /**
    * Create the specified collection.
    *
    * @param $name   the string argument. The collection name.
    *
    * @param $options an array or the string argument. The options specified by use.
    *                                                  e.g. @code
    *                                                       array( 'Compressed' => true )
    *                                                       @endcode
    *                                                  @code
    *                                                  ShardingKey         : The partition key.
    *                                                  ShardingType        : The partition type.
    *                                                  Partition           : Number of partitions, ShardingType is 'hash', represented the number of hash partitions, its value must be a power of 2, range [ 2^3, 2^20 ], default 1024.
    *                                                  ReplSize            : Copy write by default number is 1.
    *                                                  Compressed          : Data compression, default false.
    *                                                  CompressionType     : Types of compression, default 'snappy'.
    *                                                  IsMainCL            : Main partition, default false. 
    *                                                  AutoSplit           : Automatic split, defualt true.
    *                                                  Group               : To create a replication group.
    *                                                  AutoIndexId         : Collection is automatically created using the _id field is called '$id' a unique index, default true.
    *                                                  EnsureShardingIndex : Collection is automatically created using the ShardingKey contains the field names for the '$shard' index, default true.
    *                                                  AutoIncrement       : Auto increment.
    *                                                  @endcode
    *
    * @return Returns the result, default return array.
    *
    * @retval array   array( 'errno' => 0 )
    * @retval string  { "errno": 0 }
    *
    * Example:
    * @code
    * $err = $cs -> createCL( 'bar', array( 'Compressed' => true ) ) ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to create collection, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
    *
    * Example: create auto increment collection
    * @code
    * $err = $cs -> createCL( 'bar', array( 'AutoIncrement' => array( 'Field' => 'a', 'MaxValue' => 20000 ) ) ) ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to create collection, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function createCL( string $name, array|string $options = null ){}

   /**
    * Get the specified collection.
    *
    * @param $name	the string argument. The collection name.
    *
    * @return Returns a new SequoiaCL object.
    *
    * @retval SequoiaCL Object
    *
    * Example:
    * @code
    * $cl = $cs -> getCL( 'bar' ) ;
    * if( empty( $cl ) ) {
    *    $err = $db -> getLastErrorMsg() ;
    *    echo "Failed to call getCL, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function getCL( string $name ){}

   /**
    * Drop the specified collection.
    *
    * @param $name	the string argument. The collection name.
    *
    * @return Returns the result, default return array.
    *
    * @retval array   array( 'errno' => 0 )
    * @retval string  { "errno": 0 }
    *
    * Example:
    * @code
    * $err = $cs -> dropCL( 'bar' ) ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to drop collection, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function dropCL( string $name ){}

   /**
    * Rename collection name
    *
    * @param $oldName   The old collection name
    *
    * @param $newName   The new collection name
    *
    * @param $options   Reserved
    *
    * @return Returns the result, default return array.
    *
    * @retval array   array( 'errno' => 0 )
    * @retval string  { "errno": 0 }
    *
    * Example:
    * @code
    * $err = $cs -> renameCL( 'bar', 'new_bar' ) ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to rename collection, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function renameCL( string $oldName, string $newName, array|string $options = null ){}

   /**
    * Alter the specified collection space.
    *
    * @param $options   the array or string argument. The options to alter.
    *
    * @return Returns the result, default return array.
    *
    * @retval array   array( 'errno' => 0 )
    * @retval string  { "errno": 0 }
    *
    * Example:
    * @code
    * $err = $cs -> alter( array( 'PageSize' => 4096 ) ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to alter collection space, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function alter( array|string $options ){}

   /**
    * Alter the specified collection space to set domain.
    *
    * @param $options   the array or string argument. The options to alter.
    *
    * @return Returns the result, default return array.
    *
    * @retval array   array( 'errno' => 0 )
    * @retval string  { "errno": 0 }
    *
    * Example:
    * @code
    * $err = $cs -> setDomain( array( 'Domain' => 'domain' ) ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to alter collection space, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function setDomain( array|string $options ){}

   /**
    * Alter the specified collection space to remove domain.
    *
    * @return Returns the result, default return array.
    *
    * @retval array   array( 'errno' => 0 )
    * @retval string  { "errno": 0 }
    *
    * Example:
    * @code
    * $err = $cs -> removeDomain() ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to alter collection space, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function removeDomain(){}

   /**
    * Alter the specified collection space to enable capped.
    *
    * @return Returns the result, default return array.
    *
    * @retval array   array( 'errno' => 0 )
    * @retval string  { "errno": 0 }
    *
    * Example:
    * @code
    * $err = $cs -> enableCapped() ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to alter collection space, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function enableCapped(){}

   /**
    * Alter the specified collection space to disable capped.
    *
    * @return Returns the result, default return array.
    *
    * @retval array   array( 'errno' => 0 )
    * @retval string  { "errno": 0 }
    *
    * Example:
    * @code
    * $err = $cs -> disableCapped() ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to alter collection space, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function disableCapped(){}

   /**
    * Alter the specified collection space.
    *
    * @param $options   the array or string argument. The options to alter.
    *
    * @return Returns the result, default return array.
    *
    * @retval array   array( 'errno' => 0 )
    * @retval string  { "errno": 0 }
    *
    * Example:
    * @code
    * $err = $cs -> setAttributes( array( 'PageSize' => 4096 ) ;
    * if( $err['errno'] != 0 ) {
    *    echo "Failed to alter collection space, error code: ".$err['errno'] ;
    *    return ;
    * }
    * @endcode
   */
   public function setAttributes( array|string $options ){}
}
