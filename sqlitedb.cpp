#include "sqlitedb.h"
#include "glog/logging.h"
#include <stdlib.h>

SQLiteDB::SQLiteDB(QObject *parent) : QObject(parent)
{
#ifdef LOG_DEBUG_NO
    LOG(INFO) << "[DEBUG] " << __func__;
    LOG(INFO) << "[DEBUG] dbPath: " << dbPath.toStdString();
    LOG(INFO) << "[DEBUG] dbTableName: " << dbTableName.toStdString();
#endif
}

SQLiteDB::~SQLiteDB(){}

void SQLiteDB::initDatabase()
{
    QString dbType = "QSQLITE";

    db = QSqlDatabase::addDatabase(dbType);
    db.setDatabaseName(dbPath);
    LOG(INFO) << "[InitDatabase] DB Path: " << dbPath.toStdString();
}

void SQLiteDB::openDatabase()
{
    if(!db.open()){
        LOG(INFO) << "<" << __func__ << "> Could not open db! " << db.lastError().text().toStdString();
    }
    else{
        LOG(INFO) << "<" << __func__ << "> Open db successful! ";
    }
}

void SQLiteDB::closeDatabase()
{
    db.close();
    LOG(INFO) << "Closed DB";
}

void SQLiteDB::setDBPath(QString DBpath)
{
    dbPath = DBpath;
}

void SQLiteDB::setDBTable(QString DBTableName)
{
    dbTableName = DBTableName;
}

/**
 * @brief SQLiteDB::insertPerson - Chen them du lieu nguoi moi vao db
 * @param person - Thong tin nguoi moi
 * @return  - 1 neu chen thanh cong hoac nguoi nay da ton tai
 *          - 0 neu co loi
 */
int SQLiteDB::insertPerson(PersonInfo person)
{
    LOG(INFO) << "[INSERT] Inserting " << person.recognize_id.toStdString()
              << " to db...";

    ///< Check if info already exists in the DB
    QSqlQuery query_check(db);
    query_check.prepare("SELECT * FROM " + dbTableName + " WHERE id = ?;");
    query_check.addBindValue(person.recognize_id);
    if(!query_check.exec()){
        LOG(ERROR) << "<" << __func__ << "> check duplicate Fails ["
                   << db.lastError().text().toStdString() << "]";
        return 0;
    }

    while (query_check.next()) {
        LOG(WARNING) << "ID: " + person.recognize_id.toStdString() + " exist!";
        return 1;
    }

    QSqlQuery query(db);
    QString exec_str;
    exec_str = "INSERT INTO " + dbTableName
                + " (id, fullname, department, company, avatar, is_access)"
                + "VALUES (:id, :fullName, :department, :company, :avatar, :is_access);";

#ifdef LOG_DEBUG
    LOG(INFO) << "Info of: " << person.recognize_id.toStdString()
              << " - fullName: " << person.fullName.toStdString()
              << " - department: " << person.department.toStdString()
              << " - company: " << person.company.toStdString()
              << " - is_access: " << person.is_access.toStdString();

#endif

    query.prepare(exec_str);
    query.bindValue(":id", person.recognize_id);
    query.bindValue(":fullName", person.fullName);
    query.bindValue(":department", person.department);
    query.bindValue(":company", person.company);
    query.bindValue(":avatar", person.avtByteArray);
    query.bindValue(":is_access", person.is_access);

    if(query.exec()){
        LOG(INFO) << "<" << __func__ << "> Inserted successful!";
        return 1;
    }
    else{
        LOG(ERROR) << "<" << __func__ << "> Inserting Fails [" << db.lastError().text().toStdString() << "]";
        return 0;
    }
}

/**
 * @brief SQLiteDB::removePerson - Xoa thong tin nguoi khoi db
 * @param key - primary key: id
 * @return
 */
int SQLiteDB::removePerson(QString key)
{
    LOG(INFO) << "<" << __func__ << "> Removing " << key.toStdString();

    QSqlQuery query(db);
    QString exec_str;
    exec_str = "DELETE FROM " + dbTableName + " WHERE id = ?;";

    query.prepare(exec_str);
    query.addBindValue(key);
    if(query.exec()){
//        LOG(INFO) << "<" << __func__ << "> Removed successful!";
//        if(query.exec("VACUUM")){
//            LOG(INFO) << __func__ << " - VACUUM successful!";
//        }
        return true;
    }
    else{
        LOG(ERROR) << "<" << __func__ << "> Removing Fails. Err: "
                   << db.lastError().text().toStdString();
        return false;
    }

}

/**
 * @brief SQLiteDB::updateInfo - Ham cap nhat vao db thong tin nguoi da co
 * @param person - thong tin day du nguoi cap nhat
 * @return
 */
int SQLiteDB::updateInfo(PersonInfo person)
{
    LOG(INFO) << "<" << __func__ << "> Updating "
              << person.recognize_name.toStdString() << " to db...";

    ///< Check if info already exists in the DB
    QSqlQuery query_check(db);
    query_check.prepare("SELECT * FROM " + dbTableName + " WHERE id = ?;");
    query_check.addBindValue(person.recognize_id);

    //// Check if info exists
    //    if(!query_check.exec()){
//        LOG(ERROR) << "<" << __func__ << "> check duplicate Fails [" << db.lastError().text().toStdString() << "]";
//        return 0;
//    }

//    while (!query_check.next()) {
//        LOG(WARNING) << "ID: " + person.recognize_name.toStdString() + " does not exist!";
//        return 0;
//    }

    QSqlQuery query(db);
    QString exec_str;
    exec_str = "UPDATE " + dbTableName + " SET fullname = :fullName,"
                                         " department = :department, "
                                         " avatar = :avatar, "
                                         " version = :version, "
                                         " is_access = :is_access"
                                         " WHERE id = :id;";

    query.prepare(exec_str);
    query.bindValue(":fullname", person.fullName);
    query.bindValue(":department", person.department);
    query.bindValue(":avatar", person.avtByteArray);
    query.bindValue(":version", person.ver);
    query.bindValue("is_access", person.is_access);
    query.bindValue(":id", person.recognize_id);

//    query.prepare("UPDATE CTS SET FullName = 'Phan Tuấn Nghĩaasdfb' WHERE recognize_name = 'CMCTS-PHANTUANNGHIA-0695'");
    if(query.exec()){
        LOG(INFO) << "<" << __func__ << "> updating successful!";
        return true;
    }
    else{
        LOG(ERROR) << "<" << __func__ << "> updating Fails [" << db.lastError().text().toStdString() << "]";
        return false;
    }

}

#include <QFile>
#include <QTextStream>


QJsonDocument SQLiteDB::exportIDs()
{
    LOG(INFO) << "[Export IDs]. DB isOpen? " << db.isOpen();

    if(!db.open()){
        LOG(ERROR) << "<" << __func__ << "> Fail to open database: " << dbPath.toStdString();
    }

    QSqlQuery query(db);
    QString exec_str;
    exec_str = "SELECT id, version FROM " + dbTableName;

#if export_to_file
    QString listID = "listIDs.txt";
    QFile file(listID);
    file.open(QIODevice::ReadWrite);
    QTextStream stream(&file);
#else // Export to json
//    QJsonDocument jsonDoc;
    QJsonArray IDsArr;
    QJsonObject IDObject;
#endif

    query.prepare(exec_str);
    if(!query.exec()){

#ifdef LOG_DEBUG
        LOG(INFO) << "[DEBUG] dbPath: " << dbPath.toStdString();
        LOG(INFO) << "[DEBUG] dbTableName: " << dbTableName.toStdString();
#endif
        LOG(ERROR) << "<" << __func__ << "> export IDs Fails. Err: "
                   << db.lastError().text().toStdString();
    }

    while (query.next()) {
// Set if 1 to test
#if 0
        LOG(INFO) << "[DEBUG-ExportIDs] query size: " << query.size();
        for(auto value:query.){
            LOG(INFO) << query.value(i).toString().toStdString();
        }
#else
        QString recognize_id = query.value(0).toString();
        QString ver = query.value(1).toString();

        LOG(INFO) << "<" << __func__ << "> ID-ver: " << recognize_id.toStdString()
                  << " - " << ver.toStdString();
#if export_to_file
        stream << recognize_id << " " << ver << endl;
#else
        IDObject.insert("id", recognize_id);
        IDObject.insert("ver", ver);
        IDsArr.push_back(IDObject);
#endif
#endif
    }

#if export_to_file
    file.close();
#endif
    QJsonDocument jsonDoc( IDsArr );
    return jsonDoc;
}

int SQLiteDB::vacuumDB()
{
    QSqlQuery query(db);
    if(!query.exec("VACUUM")){
       LOG(ERROR) << "[DB-Update] - VACUUM fails" << db.lastError().text().toStdString();
    }
    else {
        LOG(INFO) << "[DB-Update] - VACUUM DB Done!";
    }
    return 0;
}
