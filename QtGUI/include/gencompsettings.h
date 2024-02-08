#pragma once

#include <QColor>
#include <QFont>
#include <QSettings>

namespace GenComp {

// =========== Definitions of the name of all settings within GenComp ============
// User-modifyable settings
#define GENCOMP_SETTING_REWINDSTACKSIZE ("simulator_rewindstacksize")
#define GENCOMP_SETTING_CCPATH ("compiler_path")
#define GENCOMP_SETTING_CCARGS ("compiler_args")
#define GENCOMP_SETTING_FORMATTER_PATH ("formatter_path")
#define GENCOMP_SETTING_FORMAT_ON_SAVE ("format_on_save")
#define GENCOMP_SETTING_FORMATTER_ARGS ("formatter_args")
#define GENCOMP_SETTING_LDARGS ("linker_args")
#define GENCOMP_SETTING_CONSOLEECHO ("console_echo")
#define GENCOMP_SETTING_CONSOLEBG ("console_bg_color")
#define GENCOMP_SETTING_CONSOLEFONTCOLOR ("console_font_color")
#define GENCOMP_SETTING_CONSOLEFONT ("console_font")
#define GENCOMP_SETTING_INDENTAMT ("editor_indent")
#define GENCOMP_SETTING_UIUPDATEPS ("ui_update_ps")

#define GENCOMP_SETTING_ASSEMBLER_TEXTSTART ("text_start")
#define GENCOMP_SETTING_ASSEMBLER_DATASTART ("data_start")
#define GENCOMP_SETTING_ASSEMBLER_BSSSTART ("bss_start")

#define GENCOMP_SETTING_PIPEDIAGRAM_MAXCYCLES ("pipelinediagram_maxcycles")
#define GENCOMP_SETTING_PERIPHERALS_START ("peripheral_start")
#define GENCOMP_SETTING_CACHE_MAXCYCLES ("cacheplot_maxcycles")
#define GENCOMP_SETTING_CACHE_MAXPOINTS ("cacheplot_maxpoints")
#define GENCOMP_SETTING_CACHE_PRESETS ("cache_presets")
#define GENCOMP_SETTING_PERIPHERAL_SETTINGS ("peripheral_settings")
#define GENCOMP_SETTING_VCD_TRACE ("enable_vcd_trace")
#define GENCOMP_SETTING_VCD_TRACE_FILE ("vcd_trace_file")

// This is not really a setting, but instead a method to leverage the static
// observer objects that are generated for a setting. Used for other objects to
// hook into a signal emitted just before the application closes.
#define GENCOMP_GLOBALSIGNAL_QUIT ("sig_quit")
#define GENCOMP_GLOBALSIGNAL_REQRESET ("req_reset")

// Program state preserving settings
#define GENCOMP_SETTING_SETTING_TAB ("settings_tab")
#define GENCOMP_SETTING_VIEW_ZOOM ("view_zoom")
#define GENCOMP_SETTING_PROCESSOR_ID ("processor_id")
#define GENCOMP_SETTING_PROCESSOR_EXTENSIONS ("processor_extensions")
#define GENCOMP_SETTING_PROCESSOR_LAYOUT_ID ("processor_layout_id")
#define GENCOMP_SETTING_FOLLOW_EXEC ("follow_execution")
#define GENCOMP_SETTING_INPUT_TYPE ("input_type")
#define GENCOMP_SETTING_SOURCECODE ("sourcecode")
#define GENCOMP_SETTING_DARKMODE ("darkmode")
#define GENCOMP_SETTING_SHOWSIGNALS ("show_signals")
#define GENCOMP_SETTING_AUTOCLOCK_INTERVAL ("autoclock_interval")
#define GENCOMP_SETTING_EDITORREGS ("editor_regs")
#define GENCOMP_SETTING_EDITORCONSOLE ("editor_console")
#define GENCOMP_SETTING_EDITORSTAGEHIGHLIGHTING ("editor_stage_highlighting")

#define GENCOMP_SETTING_HAS_SAVEFILE ("has_savefile")
#define GENCOMP_SETTING_SAVEPATH ("savepath")
#define GENCOMP_SETTING_SAVE_SOURCE ("save_source")
#define GENCOMP_SETTING_SAVE_BINARY ("save_binary")

// ============= Definitions of all default settings within GenComp ==============
const extern std::map<QString, QVariant> s_defaultSettings;

/**
 * @brief The SettingObserver class
 * A wrapper class around a single GenComp setting.
 * Provides a slot for setting the setting and a signal for broadcasting that
 * the setting was modified. Useful for propagating settings changes to all over
 * the codebase.
 */
class SettingObserver : public QObject {
  Q_OBJECT
  friend class GenCompSettings;

public:
  SettingObserver(const QString &key) : m_key(key) {}

  template <typename T = QVariant>
  T value() const {
    QSettings settings;
    Q_ASSERT(settings.contains(m_key));
    return settings.value(m_key).value<T>();
  }

signals:
  void modified(const QVariant &value);

public slots:
  void setValue(const QVariant &value);
  void trigger();

private:
  QString m_key;
};

class GenCompSettings : public QSettings {
public:
  static void setValue(const QString &key, const QVariant &value);
  template <typename T = QVariant>
  static T value(const QString &key) {
    return get().m_observers.at(key).value<T>();
  }

  static bool hasSetting(const QString &key) {
    return get().m_observers.count(key) != 0;
  }

  /**
   * @brief getObserver
   * returns the observer for the given setting key @p key
   */
  static SettingObserver *getObserver(const QString &key);

private:
  static GenCompSettings &get() {
    static GenCompSettings inst;
    return inst;
  }
  GenCompSettings();
  ~GenCompSettings();

  std::map<QString, SettingObserver> m_observers;
};

} // namespace GenComp
