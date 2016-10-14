#include "player.h"

Player::Player() {
    this->_name_ = "NULL";
    this->_password_ = "NULL";
    this->_pokemonNumber_ = 0;
    this->_rank_ = 9999; //TODO 初始化为总人数最后一名
    this->_beginDateTime_ = QDateTime::currentDateTime();
    this->_gameTime_ = QTime(0, 0, 0, 0);
    this->_pokemonGot_.clear();
}

Player::Player(string name, string password) {
    this->_name_ = name;
    this->_password_ = password;
    this->_pokemonNumber_ = 0;
    this->_rank_ = 9999; //TODO 初始化为总人数最后一名
    this->_beginDateTime_ = QDateTime::currentDateTime();
    this->_gameTime_ = QTime(0, 0, 0, 0);
    this->_pokemonGot_.clear();
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

QDateTime Player::getBeginDT() {
    return this->_beginDateTime_;
}

QTime Player::getGameTime() {
    return this->_gameTime_;
}

set<Pokemon> Player::getGotPokemon() {
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

void Player::setBeginDateTime(QDateTime beginDateTime) {
    this->_beginDateTime_ = beginDateTime;
}

void Player::setGameTime(QDateTime startDateTime) {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int daysDuration = currentDateTime.daysTo(startDateTime);
    int secsDuration = currentDateTime.secsTo(startDateTime);
    int minsDuration = secsDuration/ 60;
    secsDuration -= minsDuration* 60;
    int hoursDuration = minsDuration/ 60;
    minsDuration -= hoursDuration* 60;
    hoursDuration += daysDuration* 24;
    this->_gameTime_ = QTime(hoursDuration, minsDuration, secsDuration, 0);
}

void Player::addPokemon(Pokemon PMtoGet) {
    this->_pokemonGot_.insert(PMtoGet);
}
