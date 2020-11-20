#pragma once

class A
{

};

class BA
{

};

class BA1 : public BA
{
    virtual void goo() {};
};

class BA2 : public BA
{

};

class BA11 : public BA1
{

};

class BB
{

};

class BB1 : public BB
{
    virtual void foo() {};
};

class BB2 : public BB
{

};

class BB21 : public BB2
{

};

