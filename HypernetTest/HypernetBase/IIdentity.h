#pragma once

//todo разобраться почему тесты ломаются если убрать
class IIdentity
{
public:
    virtual ~IIdentity() = default;
    virtual int GetId () const = 0;
};