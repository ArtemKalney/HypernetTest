#pragma once

class IIdentity
{
public:
    virtual ~IIdentity() = default;
    virtual int GetId () const = 0;
};
