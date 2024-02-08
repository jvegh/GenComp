#include "gencompsettings.h"
//#include "assembler/program.h"
//#include "cachesim/cachesim.h"

#include <QCoreApplication>
#include <map>

namespace GenComp {

// ============= Definitions of all default settings within GenComp ==============
const std::map<QString, QVariant> s_defaultSettings = {
    // User-modifyable settings
    {GENCOMP_SETTING_REWINDSTACKSIZE, 100},
    {GENCOMP_SETTING_CCPATH, ""},
    {GENCOMP_SETTING_FORMATTER_PATH, "clang-format"},
    {GENCOMP_SETTING_FORMAT_ON_SAVE, false},
    {GENCOMP_SETTING_FORMATTER_ARGS, "--style=file --fallback-style=LLVM"},
    {GENCOMP_SETTING_CCARGS, "-O0 -g"},
    {GENCOMP_SETTING_LDARGS,
     "-static -lm"}, // Ensure statically linked executable + link with math
                     // library
    {GENCOMP_SETTING_CONSOLEECHO, "true"},
    {GENCOMP_SETTING_CONSOLEBG, QColorConstants::White},
    {GENCOMP_SETTING_CONSOLEFONTCOLOR, QColorConstants::Black},
    {GENCOMP_SETTING_CONSOLEFONT,
     QVariant() /* Let Console define its own default font */},
    {GENCOMP_SETTING_CONSOLEFONT, QColorConstants::Black},
    {GENCOMP_SETTING_INDENTAMT, 4},
    {GENCOMP_SETTING_UIUPDATEPS, 25},

    {GENCOMP_SETTING_ASSEMBLER_TEXTSTART, 0x0},
    {GENCOMP_SETTING_ASSEMBLER_DATASTART, 0x10000000},
    {GENCOMP_SETTING_ASSEMBLER_BSSSTART, 0x11000000},
    {GENCOMP_SETTING_PERIPHERALS_START, static_cast<unsigned>(0xF0000000)},
    {GENCOMP_SETTING_EDITORREGS, true},
    {GENCOMP_SETTING_EDITORCONSOLE, true},
    {GENCOMP_SETTING_EDITORSTAGEHIGHLIGHTING, true},
    {GENCOMP_SETTING_VCD_TRACE_FILE, "gencomp.vcd"},
    {GENCOMP_SETTING_VCD_TRACE, false},

    {GENCOMP_SETTING_PIPEDIAGRAM_MAXCYCLES, 100},
    {GENCOMP_SETTING_CACHE_MAXCYCLES, 10000},
    {GENCOMP_SETTING_CACHE_MAXPOINTS, 1000},
    #ifdef AA
    {GENCOMP_SETTING_CACHE_PRESETS,
     QVariant::fromValue<QList<CachePreset>>(
         {CachePreset{"32-entry 4-word direct-mapped", 2, 5, 0,
                      WritePolicy::WriteBack, WriteAllocPolicy::WriteAllocate,
                      ReplPolicy::LRU},
          CachePreset{"32-entry 4-word fully associative", 2, 0, 5,
                      WritePolicy::WriteBack, WriteAllocPolicy::WriteAllocate,
                      ReplPolicy::LRU},
          CachePreset{"32-entry 4-word 2-way set associative", 2, 4, 1,
                      WritePolicy::WriteBack, WriteAllocPolicy::WriteAllocate,
                      ReplPolicy::LRU}})},
     // Program state preserving settings
    {GENCOMP_GLOBALSIGNAL_QUIT, 0},
    {GENCOMP_GLOBALSIGNAL_REQRESET, 0},

    {GENCOMP_SETTING_SETTING_TAB, 0},
    {GENCOMP_SETTING_VIEW_ZOOM, 250},
    {GENCOMP_SETTING_PERIPHERAL_SETTINGS, ""},
    {GENCOMP_SETTING_PROCESSOR_ID,
     QVariant() /* Let processorhandler define default */},
    {GENCOMP_SETTING_PROCESSOR_EXTENSIONS,
     QVariant() /* Let processorhandler define default */},
    {GENCOMP_SETTING_PROCESSOR_LAYOUT_ID, 0},
    {GENCOMP_SETTING_FOLLOW_EXEC, "true"},
    {GENCOMP_SETTING_SOURCECODE, ""},
    {GENCOMP_SETTING_DARKMODE, true},
    {GENCOMP_SETTING_SHOWSIGNALS, false},
 //   {GENCOMP_SETTING_INPUT_TYPE, static_cast<unsigned>(SourceType::Assembly)},
    {GENCOMP_SETTING_AUTOCLOCK_INTERVAL, 100},

    {GENCOMP_SETTING_HAS_SAVEFILE, false},
    {GENCOMP_SETTING_SAVEPATH, ""},
    {GENCOMP_SETTING_SAVE_SOURCE, true},
    {GENCOMP_SETTING_SAVE_BINARY, false}
    #endif //
    };

void SettingObserver::setValue(const QVariant &v) {
  QSettings settings;
  Q_ASSERT(settings.contains(m_key));
  settings.setValue(m_key, v);

  emit modified(value());
}

void SettingObserver::trigger() { setValue(value()); }

GenCompSettings::~GenCompSettings() {}

GenCompSettings::GenCompSettings() {
  // Create a global organization & application name for QSettings to refer to
  QCoreApplication::setOrganizationName("GenComp");
  QCoreApplication::setOrganizationDomain("https://github.com/jvegh/GenComp");
  QCoreApplication::setApplicationName("GenComp");

  // Populate settings with default values if settings value is not found
  QSettings settings;
  for (const auto &setting : s_defaultSettings) {
    if (!settings.contains(setting.first)) {
      settings.setValue(setting.first, setting.second);
    }
  }

  // Create observer objects for each setting
  for (const auto &setting : s_defaultSettings) {
    m_observers.emplace(setting.first, setting.first);
  }
}

SettingObserver *GenCompSettings::getObserver(const QString &key) {
  return &get().m_observers.at(key);
}

void GenCompSettings::setValue(const QString &key, const QVariant &value) {
  get().m_observers.at(key).setValue(value);
}

} // namespace GenComp
//Q_DECLARE_METATYPE(QList<GenComp::CachePreset>);
