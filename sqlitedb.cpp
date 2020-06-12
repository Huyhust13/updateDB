#include "sqlitedb.h"
#include "glog/logging.h"

SQLiteDB::SQLiteDB(QObject *parent) : QObject(parent)
{
}

SQLiteDB::~SQLiteDB(){}

void SQLiteDB::openDatabase()
{
//    dbPath = DB_NAME;

    QString dbType = "QSQLITE";
    QSqlDatabase db = QSqlDatabase::addDatabase(dbType);
    db.setDatabaseName(dbPath);

    if(!db.open()){
        LOG(INFO) << "<" << __func__ << "> Could not open db! " << db.lastError().text().toStdString();
    }
    else{
        LOG(INFO) << "<" << __func__ << "> Open db successful! ";
    }
}

void SQLiteDB::setDBName(QString DBpath)
{
    dbPath = DBpath;
}

void SQLiteDB::setDBTable(QString DBTableName)
{
    dbTableName = DBTableName;
}

int SQLiteDB::insertPerson(PersonInfo person)
{
    LOG(INFO) << "<" << __func__ << "> Inserting " << person.recognize_name.toStdString() << " to db...";

    QSqlDatabase db = QSqlDatabase::database(dbPath);
//    QString table_name = TABLE_NAME;

    ///< Check if info already exists in the DB
    QSqlQuery query_check(db);
    query_check.prepare("SELECT * FROM " + dbTableName + " WHERE recognize_name = ?;");
    query_check.addBindValue(person.recognize_name);
    if(!query_check.exec()){
        LOG(ERROR) << "<" << __func__ << "> check duplicate Fails [" << db.lastError().text().toStdString() << "]";
        return 0;
    }

    while (query_check.next()) {
        LOG(WARNING) << "ID: " + person.recognize_name.toStdString() + " exist!";
        return 0;
    }

    QSqlQuery query(db);
    QString exec_str;
    exec_str = "INSERT INTO " + dbTableName + " (recognize_name, FullName, EmployeeID, Department, Level, Avatar)"
               + "VALUES (:recognize_name, :fullName, :employeeID, :department, :level, :avatar);";

    query.prepare(exec_str);
    query.bindValue(":recognize_name", person.recognize_name);
    query.bindValue(":fullName", person.fullName);
    query.bindValue(":employeeID", person.employeeID);
    query.bindValue(":department", person.department);
    query.bindValue(":level", person.level);
    query.bindValue(":avatar", person.avtByteArray);

    if(query.exec()){
        LOG(INFO) << "<" << __func__ << "> Inserted successful!";
        return true;
    }
    else{
        LOG(ERROR) << "<" << __func__ << "> Inserting Fails [" << db.lastError().text().toStdString() << "]";
        return false;
    }
//    return 0;
}

int SQLiteDB::removePerson(QString key)
{
    LOG(INFO) << "<" << __func__ << "> Removing " << key.toStdString();

    QSqlDatabase db = QSqlDatabase::database(dbPath);

    QSqlQuery query(db);
    QString exec_str;
//    exec_str = "DELETE FROM " + dbTableName + " WHERE recognize_name = '" + key + "';";
    exec_str = "DELETE FROM " + dbTableName + " WHERE recognize_name = ?;";

    query.prepare(exec_str);
    query.addBindValue(key);
    if(query.exec()){
        LOG(INFO) << "<" << __func__ << "> Removed successful!";
        return true;
    }
    else{
        LOG(ERROR) << "<" << __func__ << "> Removing Fails [" << db.lastError().text().toStdString() << "]";
        return false;
    }

}

int SQLiteDB::updateInfo(PersonInfo person)
{
    LOG(INFO) << "<" << __func__ << "> Updating " << person.recognize_name.toStdString() << " to db...";

    QSqlDatabase db = QSqlDatabase::database(dbPath);
//    QString table_name = TABLE_NAME;

    ///< Check if info already exists in the DB
    QSqlQuery query_check(db);
    query_check.prepare("SELECT * FROM " + dbTableName + " WHERE recognize_name = ?;");
    query_check.addBindValue(person.recognize_name);

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
    exec_str = "UPDATE " + dbTableName + " SET FullName = :fullName,"
                                         " EmployeeID = :employeeID, "
                                         "Department = :department, "
                                         "Level = :level, "
                                         "Avatar = :avatar,"
                                         " Version = :version"
                                         " WHERE recognize_name = :recognize_name;";

    query.prepare(exec_str);
    query.bindValue(":fullName", person.fullName);
    query.bindValue(":employeeID", person.employeeID);
    query.bindValue(":department", person.department);
    query.bindValue(":level", person.level);
    query.bindValue(":avatar", person.avtByteArray);
    query.bindValue(":version", person.ver);
    query.bindValue(":recognize_name", person.recognize_name);

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
#define export_to_json


QJsonDocument SQLiteDB::exportIDs()
{
    LOG(INFO) << "<" << __func__ << "> Export IDs";
    QSqlDatabase db = QSqlDatabase::database(dbPath);
    QSqlQuery query(db);
    QString exec_str;
    exec_str = "SELECT recognize_name, Version FROM " + dbTableName;

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
        LOG(ERROR) << "<" << __func__ << "> export IDs Fails [" << db.lastError().text().toStdString() << "]";
    }
    while (query.next()) {
        QString recognize_name = query.value(0).toString();
        QString ver = query.value(1).toString();

        LOG(INFO) << "<" << __func__ << "> ID-ver: " << recognize_name.toStdString() << " - " << ver.toStdString();
#if export_to_file
        stream << recognize_name << " " << ver << endl;
#else
        IDObject.insert("id", recognize_name);
        IDObject.insert("ver", ver);
        IDsArr.push_back(IDObject);
#endif
    }

#if export_to_file
    file.close();
#endif
    QJsonDocument jsonDoc( IDsArr );
    return jsonDoc;
}

void SQLiteDB::updateDB()
{
    LOG(INFO) << "<" << __func__ << "> Starting update database...";

    SQLiteDB db;


//    db.setDBName(dbPath);
//    db.setDBTable(dbTableName);
    db.openDatabase();

    uint cnt = 1;
    uint total = removed_vec.size();
    ///< REMOVE
    if(total){
        for(QString element : removed_vec){
            LOG(INFO) << "<" << __func__ << "> Removing: " << cnt << "/" << total;
            if(!db.removePerson(element)){
                removedFail_StringVec.push_back(element);
            }
            cnt++;
        }
        LOG(INFO) << "<" << __func__ << "> REMOVE: " << total - removedFail_StringVec.size() << " Successfull - " \
                  << removedFail_StringVec.size() << " Fail!";
    }
    else{
        LOG(INFO) << "<" << __func__ << "> Nothing to remove";
    }

    ///< ADD
    cnt = 1;
    total = added_vec.size();
    if(total){
        for(PersonInfo element : added_vec){
            LOG(INFO) << "<" << __func__ << "> Adding: " << cnt << "/" << total;
            if(!db.insertPerson(element)){
                addedFail_PersonVec.push_back(element);
            }
            cnt++;
        }
        LOG(INFO) << "<" << __func__ << "> ADD: " << total - addedFail_PersonVec.size() << " Successfull - " \
                  << addedFail_PersonVec.size() << " Fail!";
    }
    else{
        LOG(INFO) << "<" << __func__ << "> Nothing to add";
    }

    ///< UPDATE
    cnt = 1;
    total = updated_vec.size();
    if(total){
        for (PersonInfo person : updated_vec) {
            LOG(INFO) << "<" << __func__ << "> Updating: " << cnt << "/" << total;
            if(!db.updateInfo(person)){
                updatedFail_PersonInfoVec.push_back(person);
            }
            cnt++;
        }
        LOG(INFO) << "<" << __func__ << "> UPDATE: " << total - updatedFail_PersonInfoVec.size() << " Successfull - " \
                  << updatedFail_PersonInfoVec.size() << " Fail!";
    }
    else{
        LOG(INFO) << "<" << __func__ << "> Nothing to update";
    }

//    db.exportIDs();

}