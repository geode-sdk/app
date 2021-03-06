#pragma once

#include <Windows.h>
#include <string>
#include <utils.hpp>

#define DEF_THEME_GETTER(_var_) \
    inline static auto const& _var_() { return Style::current()->m_theme.m_##_var_; }

struct Theme {
    std::string m_id;
    std::string m_font;
    Color m_BG;
    Color m_sidebar;
    Color m_text;
    Color m_primary;
    Color m_secondary;
    Color m_tab;
    Color m_separator;
    Color m_hover;
    Color m_selectedTab;
    Color m_dark;
    Color m_inputBG;
    Color m_warning;
    Color m_error;
    Color m_launch;
    Color m_button;
    Color m_select;
    int m_buttonBorder;
    int m_buttonPress;
    int m_buttonGradient;
    bool m_useBorders;

    enum class Default {
        Light, Dark, Flat, Gay,
    };

    static Theme& get(Default);
};

class Style {
protected:
    Theme m_theme;

    Style();

public:

    static std::string const& id() { return Style::current()->m_theme.m_id; }
    static std::string const& font() { return Style::current()->m_theme.m_font; }
    DEF_THEME_GETTER(BG);
    DEF_THEME_GETTER(sidebar);
    DEF_THEME_GETTER(text);
    DEF_THEME_GETTER(primary);
    DEF_THEME_GETTER(secondary);
    DEF_THEME_GETTER(tab);
    DEF_THEME_GETTER(separator);
    DEF_THEME_GETTER(hover);
    DEF_THEME_GETTER(selectedTab);
    DEF_THEME_GETTER(dark);
    DEF_THEME_GETTER(inputBG);
    DEF_THEME_GETTER(warning);
    DEF_THEME_GETTER(error);
    DEF_THEME_GETTER(launch);
    DEF_THEME_GETTER(button);
    DEF_THEME_GETTER(select);
    DEF_THEME_GETTER(buttonBorder);
    DEF_THEME_GETTER(buttonPress);
    DEF_THEME_GETTER(buttonGradient);
    DEF_THEME_GETTER(useBorders);

    static Style* current();
    void load(Theme const& theme);
};
