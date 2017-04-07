#pragma once
#include "includes/plugins/editor.h"

class EditorFactory
{
public:
    static qnoto::EditorInstance* create(const QString& fname);
};
