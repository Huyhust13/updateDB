#include <glog/logging.h>
#include "update.h"
#include <QFile>
#include <QDebug>



Update::Update()
{

}

/**
 * @brief Update::readJsonFromFile
 * @param fileName: json file that involed info to update database
 * @return
 *
 * Read json from file
 *      + push Person's info to add vector
 *      + push Person's recognition_name (id) to removed vector
 *      + push Person's info to updated vector
 */
int Update::readJsonFromFile(QString fileName)
{
    std::ifstream inFileJson(fileName.toStdString());

    json j = json::parse(inFileJson);
//    LOG(INFO) << "<" << __func__ << "> " << j["add"][1]["avt"];
    LOG(INFO) << "<" << __func__ << "> Add " << j["add"].size() << " people to DB";

    // push info need to add to added_vec<PersonInfo>
    LOG(INFO) << "Push to added_vec<PersonInfo>";
    for (uint i=0; i < j["add"].size(); i++) {
        PersonInfo person;
        json json_person = j["add"][i];
        try {
            if(json_person["id"] != nullptr){
                person.recognize_name = QString::fromStdString(json_person["id"]);
            }
            else{
                throw;
            }

            if(json_person["fullName"] != nullptr){
                person.fullName = QString::fromStdString(json_person["fullName"]);
            }
            else{
                throw;
            }

            if(json_person["department"] != nullptr){
                person.department = QString::fromStdString(json_person["department"]);
            }
            else{
                person.department = "UNKNOWN";
            }

            if(json_person["level"] != nullptr){
                person.level = QString::fromStdString(json_person["level"]);
            }
            else{
                person.level = 1;
            }

            if(json_person["avt"] != nullptr){
                QByteArray base64Avt = QByteArray::fromStdString(json_person["avt"]);
//                QImage avt_img;
//                avt_img.loadFromData(QByteArray::fromBase64(base64Avt));
//                person.avt = avt_img;
                person.avtByteArray = base64Avt;
#if 0
                LOG(INFO) << person.avt.bits();
#endif
            }

            if(json_person["ver"] != nullptr){
                person.ver = json_person["ver"];
            }

            added_vec.push_back(person);
            LOG(INFO) << "<" << __func__ << "> " << i << " - recognize_name" << j["add"][i]["id"];

        } catch (json::exception &e) {
            LOG(ERROR) << e.what();
            addedFail_jsonVec.push_back(json_person);
            LOG(INFO) << "<" << __func__ << "> Have " << addedFail_jsonVec.size() << " json added Fail";
        }

    }

    LOG(INFO)<< "push recognize_name need to remove to removed_vec";
    for (uint i=0; i < j["remove"].size(); i++){
        removed_vec.push_back(QString::fromStdString(j["remove"][i]));
        LOG(INFO) << "<" << __func__ << "> Have " << removed_vec.size() << " have to remove";
//        LOG(INFO) << "add remove: " << j["remove"][i];
    }


    ///< ADD UPDATE TO VECTOR
    LOG(INFO) << "Push to added_vec<PersonInfo>";
    for (uint i=0; i < j["update"].size(); i++) {
        PersonInfo person;
        json json_person = j["update"][i];
        try {
            if(json_person["id"] != nullptr){
                person.recognize_name = QString::fromStdString(json_person["id"]);
            }
            else{
                throw;
            }

            if(json_person["fullName"] != nullptr){
                person.fullName = QString::fromStdString(json_person["fullName"]);
            }
            else{
                throw;
            }

            if(json_person["department"] != nullptr){
                person.department = QString::fromStdString(json_person["department"]);
            }
            else{
                person.department = "UNKNOWN";
            }

            if(json_person["level"] != nullptr){
                person.level = QString::fromStdString(json_person["level"]);
            }
            else{
                person.level = 1;
            }

            if(json_person["avt"] != nullptr){
                QByteArray base64Avt = QByteArray::fromStdString(json_person["avt"]);
//                QImage avt_img;
//                avt_img.loadFromData(QByteArray::fromBase64(base64Avt));
//                person.avt = avt_img;
                person.avtByteArray = base64Avt;
#if 0
                LOG(INFO) << person.avt.bits();
#endif
            }

            if(json_person["ver"] != nullptr){
                person.ver = json_person["ver"];
            }

            updated_vec.push_back(person);

        } catch (json::exception &e) {
            LOG(ERROR) << e.what();
            updatedFail_vec.push_back(json_person);
            LOG(INFO) << "<" << __func__ << "> Had " << addedFail_jsonVec.size() << " info add to updated Fail";
        }

    }


    return 0;
}

/**
 * @brief Update::updateDB
 * @return
 */
int Update::updateDB(QString dbPath, QString dbTableName)
{
    LOG(INFO) << "<" << __func__ << "> Starting update database...";

    SQLiteDB db;


    db.setDBName(dbPath);
    db.setDBTable(dbTableName);
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

    return 1;
}


