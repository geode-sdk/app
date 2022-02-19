#pragma once

#include "Widget.hpp"

class Button : public TextWidget {
public:
    using Callback = std::function<void(Button*)>;

protected:
    COLORREF m_bgColor;
    Callback m_callback;

    void paint(HDC hdc, PAINTSTRUCT* ps) override;

public:
    Button(std::string const& text);
    virtual ~Button();

    bool wantsMouse() const override;

    void enter() override;
    void leave() override;
    HCURSOR cursor() const;

    void setBG(COLORREF);
    COLORREF getBG() const;

    void click() override;
    void setCallback(Callback cb);
};
