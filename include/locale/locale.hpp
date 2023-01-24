// MIT License
//
// Copyright (c) 2023 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//

#pragma once

#define __LOCALE_SIZE__ 10
#define __LOCALE_EN__ "en_US.UTF-8"

enum analyzer_callback_type {
    BuiltInCall = 0,
    ReturnWith,
    UserDefinedCall,
    IfElifLoopIfLoopElifCheck,
    ElseInterpret,
    ExitWith,
    InitializeVariant,
    DefineArray,
    AccessWithIndexN,
    EvaluateExpr
};
