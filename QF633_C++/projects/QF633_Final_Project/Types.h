#ifndef TYPES_H
#define TYPES_H

enum OptionType 
{   
    Call, 
    Put, 
    BinaryCall, 
    BinaryPut,
    None
};

enum DirectionType {
    Pay,
    Receive,
    Long,
    Short,
    NoneDir
};

#endif
