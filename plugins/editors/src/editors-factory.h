#pragma once
#include "includes/plugins/editor.h"

class EditorsFactory
{
public:
    static qnoto::EditorInstance* create(const QString& fname);
};
