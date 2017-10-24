#include "includes/plugin.h"

namespace qnoto {

Plugin::Plugin()
{}

Plugin::~Plugin()
{}

bool Plugin::saveState() const
{ return true; }

void Plugin::restoreState()
{}

PreferencesPage* Plugin::preferences() const
{ return nullptr; }

}
