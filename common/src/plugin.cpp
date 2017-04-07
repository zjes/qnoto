#include "includes/plugin.h"

namespace qnoto {

Plugin::Plugin()
{}

Plugin::~Plugin()
{}

void Plugin::saveState(QSettings& /*sets*/) const
{}

void Plugin::restoreState(QSettings& /*sets*/)
{}

PreferencesPage* Plugin::preferences() const
{ return nullptr; }

}
