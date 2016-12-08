#include "player.h"
#include <string>

Player::Player() {
    this->_name_ = "NULL";
    this->_password_ = "NULL";
    this->_pokemonNumber_ = 0;
    this->_rank_ = 9999; //TODO 初始化为总人数最后一名
    this->_packageCapacity_ = CAPACITY;
    this->_thumb_ = 0;
    this->_beginDateTime_ = QDateTime::currentDateTime();
    this->_gameTime_ = "0000";
    this->_pokemonGot_.clear();
}

Player::Player(PlayerInfo playerInfo) {
    this->_name_ = playerInfo.name;
    this->_password_ = playerInfo.password;
    this->_pokemonNumber_ = playerInfo.pokemonNumber;
    this->_packageCapacity_ = playerInfo.packageCapacity;
    this->_rank_ = playerInfo.rank;
    this->_thumb_ = playerInfo.thumb;

    //set Begin Time
    string beginYear = playerInfo.beginDateTime.substr(0, 4);
    string beginMonth = playerInfo.beginDateTime.substr(4, 2);
    string beginDay = playerInfo.beginDateTime.substr(6, 2);
    string beginHour = playerInfo.beginDateTime.substr(8, 2);
    string beginMin = playerInfo.beginDateTime.substr(10,2);
    QDate beginDate;
    beginDate.setDate(stoi(beginYear), stoi(beginMonth),
                      stoi(beginDay));
    QTime beginTime;
    beginTime.setHMS(stoi(beginHour),
                     stoi(beginMin), 0);
    this->_beginDateTime_.setDate(beginDate);
    this->_beginDateTime_.setTime(beginTime);

    //set Game Time
    string reverseGameTime(playerInfo.gameTime.rbegin(),
                           playerInfo.gameTime.rend());
    string gameMin = reverseGameTime.substr(0, 2);
    gameMin = string(gameMin.rbegin(), gameMin.rend());
    reverseGameTime.erase(0, 2);
    string gameHour = string(reverseGameTime.rbegin(),
                      reverseGameTime.rend());
    this->_gameTime_ = gameHour + gameMin;

    this->_pokemonGot_.clear(); //pokemon would be added in
}

struct PlayerInfo Player::ToPlayerInfo()
{
    struct PlayerInfo playerinfo = {
        this->_name_, this->_password_, this->_pokemonNumber_,
        this->_packageCapacity_, this->_rank_, this->_thumb_,
        this->_beginDateTime_.toString("yyyyMMddhhmm").toStdString(),
        this->_gameTime_
    };
    return playerinfo;
}

string Player::getName() {
    return this->_name_;
}

string Player::getPassword() {
    return this->_password_;
}

int Player::getPMNumber() {
    return this->_pokemonNumber_;
}

int Player::getRank() {
    return this->_rank_;
}

int Player::getThumb()
{
    return this->_thumb_;
}

QDateTime Player::getBeginDT() {
    return this->_beginDateTime_;
}

string Player::getGameTime() {
    return this->_gameTime_;
}

set<Pokemon*> Player::getGotPokemon() {
    return this->_pokemonGot_;
}

void Player::setName(string name) {
    this->_name_ = name;
}

void Player::setPassword(string password) {
    this->_password_ = password;
}

void Player::setPMNumber(int pokemonNumber) {
    this->_pokemonNumber_ = pokemonNumber;
}

void Player::setRank(int rank) {
    this->_rank_ = rank;
}

void Player::setThumb(int thumb) {
    this->_thumb_ = thumb;
}

void Player::setBeginDateTime(QDateTime beginDateTime) {
    this->_beginDateTime_ = beginDateTime;
}

void Player::setGameTime(QDateTime startDateTime) {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int daysDuration = startDateTime.daysTo(currentDateTime);
    int secsDuration = startDateTime.secsTo(currentDateTime);
    std::cout << "secsDuration:" << secsDuration << std::endl;
    int minsDuration = secsDuration/ 60;
    int hoursDuration = minsDuration/ 60;
    minsDuration -= hoursDuration* 60;
    hoursDuration += daysDuration* 24;
    this->_gameTime_ = to_string(hoursDuration) + to_string(minsDuration);
}

void Player::addPokemon(Pokemon *PMtoGet) {
    this->_pokemonGot_.insert(PMtoGet);
}
