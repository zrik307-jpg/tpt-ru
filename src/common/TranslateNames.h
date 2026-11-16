#pragma once
#include "common/String.h"

// Display-only Russian names for element / tool / GOL buttons and the HUD.
// Internal names (used by Lua, the console and saves) stay English.
String TranslateToolName(String const &name);
// Same, lowercased (Cyrillic-aware) for search matching.
String TranslateToolNameLower(String const &name);
