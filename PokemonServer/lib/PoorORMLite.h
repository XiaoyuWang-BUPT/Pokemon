#ifndef POOR_ORMLITE_H
#define POOR_ORMLITE_H
#pragma once

#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <iostream>
#include <sstream>
#include <functional>
#include <vector>
#include <cctype>
#include <tuple>

#include "sqlite3.h"
#include "helper.h"

#define ORMAP(_MY_CLASS_, ...)				\
private:                                                \
friend class Poor_ORM::ORMapper<_MY_CLASS_>;		\
template <typename VISITOR>				\
void __Accept(VISITOR &visitor)				\
{														\
        visitor.Visit(__VA_ARGS__);			\
}														\
template <typename VISITOR>				\
void __Accept(VISITOR &visitor) const			\
{														\
        visitor.Visit(__VA_ARGS__);			\
}							\
constexpr static const char *tableName = #_MY_CLASS_;	\
constexpr static const char *fieldNames = #__VA_ARGS__;	\

namespace Poor_ORM_Impl
{
    class SQLConnector
    {
	public:
		SQLConnector(std::string &dbName)
		{
			int rc;
			rc = sqlite3_open(dbName.c_str(), &db);
			if (rc)
			{
				throw std::runtime_error(
					/*std::string("can not open database: %s\n") +
						sqlite3_errmsg(db)*/
					std::string("Can Not open database: ") + sqlite3_errmsg(db)
				);
			}
//			else
//				std::cout << "Open database succedd" << std::endl;
		}

		~SQLConnector()
		{
			sqlite3_close(db);
		}

		bool Execute(const std::string sqlCmd,
			std::function<void (int argc, char **argv,
				char **azCollName)> callback = _callback)
		{
			int rc;
			char *zErrMsg;
			
			auto callbackWrapper = [](void *fn, int argc, char**argv,
				char **azColName)
			{
				static_cast<std::function <void(int argc, char **argv,
					char **azColName)>*>(fn)->operator()(argc, argv, azColName);
				return 0;
			};

			rc = sqlite3_exec(db, sqlCmd.c_str(), callbackWrapper, &callback, &zErrMsg);
            if (rc != SQLITE_OK)
			{
				throw std::runtime_error(
					std::string("SQL Error(Execute): ") + zErrMsg
				);
				sqlite3_free(zErrMsg);
				return false;
			}
//			else
//				std::cout << "Execute Succedd" << std::endl;
			return true;
		}
	private:
		sqlite3* db;
		static void _callback(int argc, char **argv, char **azColName)
		{
			return;
		}
	};

	//Visitor Creating Table
	class CreateVisitor
	{
	public:
		std::string serializedTypes;
		template<typename ... Args>
		void Visit(Args & ... args)
		{
			_Visit(args...);
		}
	private:
		template<typename T, typename ... Args>
		void _Visit(T &property, Args & ... args)
		{
			_Visit(property);
			_Visit(args...);
		}

		void _Visit(const int &property)
		{
			serializedTypes += "integer";
			serializedTypes += ",";
		}
		void _Visit(const double &property)
		{
			serializedTypes += "real";
			serializedTypes += ",";
		}
		void _Visit(const std::string &property)
		{
			serializedTypes += "text";
			serializedTypes += ",";
		}
	};

	//Visitor writing into Table (Insert & Update)
	class WriteVisitor
	{
	public:
		std::string serializedValues;
		template <typename ... Args>
		void Visit(Args & ... args)
		{
			_Visit(args...);
		}
	private:
		template <typename T, typename ... Args>
		void _Visit(T &property, Args & ... args)
		{
			_Visit(property);
			_Visit(args...);
		}

		void _Visit(void)
		{}

		void _Visit(const int &property)
		{
			serializedValues += std::to_string(property);
			serializedValues += ",";
		}
		void _Visit(const double &property)
		{
			serializedValues += std::to_string(property);
			serializedValues += ",";
		}
		void _Visit(const std::string &property)
		{
			serializedValues += ("'" + property + "'");
			serializedValues += ",";
		}
	};

	class HelpVisitor
	{
	public:
		bool fieldFound;
		size_t index;

		HelpVisitor(const void *pointer): fieldFound(false), _typePointer(pointer){}

		template <typename ... Args>
		void Visit(Args & ... args)
		{
			_Visit(args...);
		}
	private:
		const void* _typePointer;
		template<typename T, typename ... Args>
		void _Visit(T &property, Args & ... args)
		{
			_Visit(property);
			if (!fieldFound)
				_Visit(args...);
		}

		void _Visit(const int &property)
		{
			if ((const void*) &property == _typePointer)
				fieldFound = true;
			else if (!fieldFound)
				index++;
		}
		void _Visit(const double &property)
		{
			if ((const void*) &property == _typePointer)
				fieldFound = true;
			else if(!fieldFound)
				index++;
		}
		void _Visit(const std::string &property)
		{
			if ((const void*) &property == _typePointer)
                            fieldFound = true;
			else if (!fieldFound)
                            index++;
		}
	};

	class ReadVisitor
	{
	public:
		std::string _serializedValues;
		ReadVisitor(std::string serializedValues)
			:_serializedValues(serializedValues){}

		template <typename ... Args>
		void Visit(Args & ... args)
		{
			_Visit(args...);
		}
	private:
		template <typename T, typename ... Args>
		void _Visit(T &property, Args & ... args)
		{
			_Visit(property);
			_Visit(args...);
		}

		void _Visit(int &property)
		{
			property = std::stoi(
                                Helper::SplitStr(_serializedValues)
			);
		}
		void _Visit(double &property)
		{
			property = std::stod(
                                Helper::SplitStr(_serializedValues)
			);
		}
		void _Visit(std::string &property)
		{
                        property = Helper::SplitStr(_serializedValues);
		}
	};
}

namespace Poor_ORM
{
	class Expr
	{
	private:
		Expr And_Or(const Expr &right, std::string logOp)
		{
			expr.emplace(expr.begin(),
				std::make_tuple(nullptr, std::string(""), std::string("(")));
			expr.emplace_back(std::make_tuple(nullptr, std::string(""), std::move(logOp)));
			for (const auto exprRight : right.expr)
				expr.emplace_back(exprRight);
			expr.emplace_back(std::make_tuple(nullptr, std::string(""), std::string(")")));

			return *this;
		}
	public:
		std::vector<std::tuple<const void*, std::string, std::string>> expr;

		template <typename T>
		Expr(){}

		Expr(const int &property, 
			const std::string &relOp,
			const int &value) :
			expr{std::make_tuple(&property, relOp, std::to_string(value))} {}
		Expr(const double &property,
			const std::string &relOp,
			const double &value) :
			expr{std::make_tuple(&property, relOp, std::to_string(value))} {}
		Expr(const std::string &property,
			const std::string &relOp,
			const std::string &value) :
			expr{ std::make_tuple(&property, relOp, "'" + value + "'")} {}
		template <typename T>
		class Expr_Field
		{
		private:
			const T &_property;
		public:
			Expr_Field(const T&property)
			:_property(property)
			{}

			inline Expr operator == (T value){ return Expr(_property, "=", std::move(value)); }
			inline Expr operator != (T value) { return Expr(_property, "!=", std::move(value)); }
			inline Expr operator < (T value) { return Expr(_property, "<", std::move(value)); }
			inline Expr operator <= (T value) { return Expr(_property, "<=", std::move(value)); }
			inline Expr operator > (T value) { return Expr(_property, ">", std::move(value)); }
			inline Expr operator >= (T value) { return Expr(_property, ">=", std::move(value)); }

		};

		inline Expr operator && (const Expr &right)
		{
			return And_Or(right, " and ");
		}
		inline Expr operator || (const Expr &right)
		{
			return And_Or(right, " or ");
		}
	};

	template <typename T>
	Expr::Expr_Field<T> Field(T &property)
	{
		return Expr::Expr_Field<T> {property};
	}

	template <typename C>
	class ORMapper
	{
	public:
		ORMapper(const std::string &dbName)
			:_dbName(dbName), _tblName(C::tableName), _fieldNamesVec(_ExtractFieldNames()) //TODO: ExtractFieldNames
		{}
		bool CreateTable()
		{
			Poor_ORM_Impl::SQLConnector connector(_dbName);
			std::string sqlCmd;
			const C obj{};
			Poor_ORM_Impl::CreateVisitor visitor;
			obj.__Accept(visitor);
			
			size_t indexField = 0;
			auto strTypes = std::move(visitor.serializedTypes); // "integer,double,string,"
                        auto strType = Helper::SplitStr(strTypes);	// "integer"/ "double"/ "sting"
			auto partSqlCmd = _fieldNamesVec[indexField++] + " " + // "id int primary key not null,"
				std::move(strType) + " primary key not null,";
			while (!strTypes.empty())
			{
                                strType = Helper::SplitStr(strTypes);
				partSqlCmd += _fieldNamesVec[indexField++] + " " +  //"id int primary key not null,
					std::move(strType) + " not null,";				// score real not null,
			}
			partSqlCmd.pop_back();
			sqlCmd = "create table if not exists " + _tblName + "(" +
				partSqlCmd + ");";
			return connector.Execute(sqlCmd);
		}
		bool DropTable()
		{
			Poor_ORM_Impl::SQLConnector connector(_dbName);
			auto sqlCmd = "drop table " + _tblName + ";";
			return connector.Execute(sqlCmd);
		}
		bool Insert(const C &value) //value: every line to insert into table
		{
			Poor_ORM_Impl::SQLConnector connector(_dbName);
			Poor_ORM_Impl::WriteVisitor visitor;
			value.__Accept(visitor);

			std::string sqlCmd;
			std::string strTypes = C::fieldNames;
			std::string strValue = std::move(visitor.serializedValues);
			visitor.serializedValues = "";
			strValue.pop_back();
			sqlCmd = "insert into " + _tblName + " (" +
				strTypes + ") select " + strValue +
                                " where not exists ( select * from " + _tblName + " where " + Helper::getPrime(strTypes) +"="
                                + Helper::getPrime(strValue) +");";
			return connector.Execute(sqlCmd);
		}
		bool Update(const C &value)
		{
			Poor_ORM_Impl::SQLConnector connector(_dbName);
			Poor_ORM_Impl::WriteVisitor visitor;
			value.__Accept(visitor);

			size_t indexField = 0;
			std::string sqlCmd;
			std::string strValue = std::move(visitor.serializedValues);
			std::string partSqlCmd = "";

			std::string PrKey = _fieldNamesVec[indexField++]; //Get Primary Key :"ID"
                        auto PrKeyValue = Helper::SplitStr(strValue); //Get Primary Key Value :"value under ID";
			auto strCon = std::move(PrKey) + "=" + std::move(PrKeyValue);

			while (!strValue.empty())
			{
				partSqlCmd += _fieldNamesVec[indexField++] + "=" +
                                        Helper::SplitStr(strValue) + ",";
			}
			partSqlCmd.pop_back();
			
			sqlCmd = "update " + _tblName + " set " + std::move(partSqlCmd) +
				" where (" + std::move(strCon) + ");";
			return connector.Execute(sqlCmd);
		}
		bool Delete(const C &value)
		{
			Poor_ORM_Impl::SQLConnector connector(_dbName);
			Poor_ORM_Impl::WriteVisitor visitor;
			value.__Accept(visitor);

			auto PrKey = _fieldNamesVec[0];
			auto strValue = std::move(visitor.serializedValues);
                        auto PrKeyValue = Helper::SplitStr(strValue);
			std::move(strValue);

			auto sqlCmd = "delete from " + _tblName + " where " +
				PrKey + "=" + PrKeyValue + ";";
			return connector.Execute(sqlCmd);
		}

                /*static*/ std::string getTblName()
		{
			return _tblName;
		}

		class ORMQuery
		{
		private:
			std::vector<C> _out;
			std::string _sqlWhere;
			std::string _sqlOrderBy;
			std::string _sqlLimit;
			const C *_qHelper;
			ORMapper<C> *_pMapper;

			std::string _GetFieldName(const void *property)
			{
				Poor_ORM_Impl::HelpVisitor visitor (property);
				(*_qHelper).__Accept(visitor);

				if (!visitor.fieldFound)
					throw std::runtime_error("No such Field in Table");
				return _pMapper->_fieldNamesVec[visitor.index];
			}
			std::string _GetSQL() const
			{
				if (!_sqlWhere.empty())
					return "select * from ( select * from (select * from " + _pMapper->_tblName + " where " +
					_sqlWhere + ")" + _sqlOrderBy + ")" + _sqlLimit + ";";
				else if ((!_sqlOrderBy.empty()) && (!_sqlLimit.empty()))
					return "select * from (select * from " + _pMapper->_tblName +
					_sqlOrderBy + " )" + _sqlLimit + ";";
				else if (!_sqlLimit.empty())
					return "select * from " + _pMapper->_tblName + _sqlLimit;
				else
					return "select * from " + _pMapper->_tblName + _sqlOrderBy;
			}
		public:
			ORMQuery(const C &qHelper, ORMapper<C> *pMapper)
			:_qHelper(&qHelper), _pMapper(pMapper)
			{}

			ORMQuery &Where(const Expr &expr)
			{
				for (const auto exprStr : expr.expr)
				{
					if (std::get<0>(exprStr) != nullptr)
						_sqlWhere += _GetFieldName(std::get<0>(exprStr));
					_sqlWhere += std::get<1>(exprStr) + std::get<2>(exprStr);
				}
				return *this;
			}
			template <typename T>
			ORMQuery &OrderBy(const T &property, bool decreasing = false)
			{
				_sqlOrderBy = " order by " + _GetFieldName(&property);
				if (decreasing)
					_sqlOrderBy += " desc";
				return *this;
			}
			ORMQuery &Limit(size_t count, size_t offset = 0)
			{
				_sqlLimit = " limit " + std::to_string(count) +
					" offset " + std::to_string(offset);
				return *this;
			}
			std::vector<C> ToVector()
			{
				std::vector<C> ret;
				_pMapper->_Select(ret, _GetSQL());
				return std::move(ret);
			}
		};
		ORMQuery Query(C &qHelper)
		{
			return ORMQuery(qHelper, this);
		}
	private:
		std::string _dbName;
		std::string _tblName;
		const std::vector<std::string> _fieldNamesVec;
		std::vector<std::string> _ExtractFieldNames()
		{
			std::vector <std::string> fieldNameVec;
			std::string tmpFieldNames = C::fieldNames; //fieldNames: "id, score, name"
			tmpFieldNames.push_back(','); //tmpFieldNames: "id, score, name,"
			std::string tmpFieldName = "";
			for (auto ch : tmpFieldNames)
			{
				if (ch == ',')
				{
					fieldNameVec.push_back(tmpFieldName);
					tmpFieldName.clear();
				}
				else
				{
					if (isalnum(ch))
					{
						tmpFieldName += ch;
					}
				}
			}
			return std::move(fieldNameVec);
		}
		template <typename Out>
		bool _Select(Out &out, const std::string &sqlStr = "")
		{
//			std::cout << sqlStr << std::endl;
			Poor_ORM_Impl::SQLConnector connector(_dbName);
			return connector.Execute(sqlStr,
				[&](int argc, char **argv, char **)
			{
				std::string serialized;
				for (int i = 0; i < argc; i++)
				{
					serialized += argv[i];
					serialized += ',';
				}
				C obj;
				Poor_ORM_Impl::ReadVisitor visitor(serialized);
				obj.__Accept(visitor);
				out.push_back(std::move(obj));
			});
		}
	};
}
#endif //!POOR_ORMLITE_H
