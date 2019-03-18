package com.sequoiadb.jdbc;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.SQLWarning;

import com.sequoiadb.base.DBCollection;
import com.sequoiadb.base.DBCursor;
import com.sequoiadb.base.Sequoiadb;

public class Statement implements java.sql.Statement{

	protected Sequoiadb sdb = null;
	
	public Statement(Sequoiadb sdb){
		
		this.sdb = sdb;
	}
	
	
	public <T> T unwrap(Class<T> iface) throws SQLException {
		return null;
	}

	public boolean isWrapperFor(Class<?> iface) throws SQLException {
		return false;
	}

	public java.sql.ResultSet executeQuery(String sql) throws SQLException {
		DBCursor dbcursor = null;
		if(sdb != null){
		 dbcursor = sdb.exec(sql);
		}
		com.sequoiadb.jdbc.ResultSet rs =	new com.sequoiadb.jdbc.ResultSet(dbcursor);
		return rs;
	}

	public int executeUpdate(String sql) throws SQLException {
		return 0;
	}

	public void close() throws SQLException {
		
	}

	public int getMaxFieldSize() throws SQLException {
		return 0;
	}

	public void setMaxFieldSize(int max) throws SQLException {
		
	}

	public int getMaxRows() throws SQLException {
		return 0;
	}

	public void setMaxRows(int max) throws SQLException {
		
	}

	public void setEscapeProcessing(boolean enable) throws SQLException {
		
	}

	public int getQueryTimeout() throws SQLException {
		return 0;
	}

	public void setQueryTimeout(int seconds) throws SQLException {
		
	}

	public void cancel() throws SQLException {
		
	}

	public SQLWarning getWarnings() throws SQLException {
		return null;
	}

	public void clearWarnings() throws SQLException {
		
	}

	public void setCursorName(String name) throws SQLException {
		
	}

	public boolean execute(String sql) throws SQLException {
		return false;
	}

	public ResultSet getResultSet() throws SQLException {
		return null;
	}

	public int getUpdateCount() throws SQLException {
		return 0;
	}

	public boolean getMoreResults() throws SQLException {
		return false;
	}

	public void setFetchDirection(int direction) throws SQLException {
		
	}

	public int getFetchDirection() throws SQLException {
		return 0;
	}

	public void setFetchSize(int rows) throws SQLException {
		
	}

	public int getFetchSize() throws SQLException {
		return 0;
	}

	public int getResultSetConcurrency() throws SQLException {
		return 0;
	}

	public int getResultSetType() throws SQLException {
		return 0;
	}

	public void addBatch(String sql) throws SQLException {
		
	}

	public void clearBatch() throws SQLException {
		
	}

	public int[] executeBatch() throws SQLException {
		return null;
	}

	public Connection getConnection() throws SQLException {
		return null;
	}

	public boolean getMoreResults(int current) throws SQLException {
		return false;
	}

	public ResultSet getGeneratedKeys() throws SQLException {
		return null;
	}

	public int executeUpdate(String sql, int autoGeneratedKeys)
			throws SQLException {
		return 0;
	}

	public int executeUpdate(String sql, int[] columnIndexes)
			throws SQLException {
		return 0;
	}

	public int executeUpdate(String sql, String[] columnNames)
			throws SQLException {
		return 0;
	}

	public boolean execute(String sql, int autoGeneratedKeys)
			throws SQLException {
		return false;
	}

	public boolean execute(String sql, int[] columnIndexes) throws SQLException {
		return false;
	}

	public boolean execute(String sql, String[] columnNames)
			throws SQLException {
		return false;
	}

	public int getResultSetHoldability() throws SQLException {
		return 0;
	}

	public boolean isClosed() throws SQLException {
		return false;
	}

	public void setPoolable(boolean poolable) throws SQLException {
		
	}

	public boolean isPoolable() throws SQLException {
		return false;
	}

	public void closeOnCompletion() throws SQLException {
		
	}

	public boolean isCloseOnCompletion() throws SQLException {
		return false;
	}

}
