#include "Tab.hpp"
#include <array>

int Tab::s_pad = 15_px;
int Tab::s_height = 32_px;
int Tab::s_dot = 9_px;
int Tab::s_arrow = 9_px;

void Tabs::add(Widget* w) {
    if (w == m_layout) Widget::add(w);
    auto t = dynamic_cast<Tab*>(w);
    if (t) {
        t->m_control = this;
        m_tabs.push_back(t);
        if (m_tabs.size() == 1) {
            t->click();
        }
    }
    m_layout->add(w);
}

void Tabs::onSelect(SelectFunc func) {
    m_func = func;
}

Tabs::Tabs(Layout* l) {
    m_layout = l;
    this->add(m_layout);
    this->show();
}

void Tabs::select(Tab* tab) {
    for (auto& t : m_tabs) {
        t->m_selected = t == tab;
        if (t == tab && t->m_id && m_func) {
            m_func(t->m_id);
        }
        t->update();
    }
}

Separator::Separator(bool p, int size, int drawSize) {
    this->drawSize(drawSize);
    this->size(size);
    this->pad(p);
    this->autoResize();
    this->show();
}

void Separator::updateSize(HDC hdc, SIZE size) {
    Widget::updateSize(hdc, size);
    if (m_autoresize) {
        this->resize(size.cx, m_size);
        m_autoresize = true;
    }
}

void Separator::paint(HDC hdc, PAINTSTRUCT* ps) {
    Graphics g(hdc);
    InitGraphics(g);

    auto r = this->rect();
    r.Y += r.Height / 2 - m_drawSize / 2;
    r.Height = m_drawSize;
    r.X += (m_pad ? Tab::s_pad : 0);
    r.Width -= 2 * (m_pad ? Tab::s_pad : 0);
    auto rf = toRectF(r);
    if (rf.Height == 1) rf.Height /= 2;
    g.FillRectangle(&SolidBrush(Style::separator()), rf);

    Widget::paint(hdc, ps);
}

void Separator::pad(bool p) {
    m_pad = p;
    this->update();
}

void Separator::size(int s) {
    m_size = s;
    this->update();
}

void Separator::drawSize(int s) {
    m_drawSize = s;
    this->update();
}

Tab::Tab(size_t id, std::string const& text, Tab::Type type) : m_id(id) {
    m_dotColor = Tab::dot();
    m_type = type;
    this->text(text);
    this->color(Style::text());
    this->autoResize();
    this->fontSize(16_px);
    this->show();
}

Tab::Tab(std::string const& text, Tab::Type type) : Tab(0, text, type) {}

void Tab::updateSize(HDC hdc, SIZE size) {
    Widget::updateSize(hdc, size);
    if (m_autoresize) {
        this->resize(size.cx, Tab::s_height);
        m_autoresize = true;
    }
}

Color Tab::dot() {
    static int ix = -1;
    std::array<Color, 8> s = {
        0xFF60BDFF,
        0xFFCA60FF,
        0xFF60FF88,
        0xFFE7FF60,
        0xFF606EFF,
        0xFFFF6E60,
        0xFFA8FF60,
        0xFFFF60A8,
    };
    ix++;
    if (ix >= s.size()) {
        ix = 0;
    }
    return s[ix];
}

void Tab::callback(Callback cb) {
    m_callback = cb;
}

void Tab::arrow(bool a) {
    m_arrow = a;
    this->update();
}

void Tab::makeButton(bool b) {
    m_button = b;
    m_arrow = !b;
    this->update();
}

void Tab::paint(HDC hdc, PAINTSTRUCT* ps) {
    auto fullRect = this->rect();
    auto r = fullRect;
    r.X += Tab::s_pad;
    r.Width -= 2 * Tab::s_pad;

    auto tr = r;
    tr.X += (s_height - Tab::s_dot) / 2 + Tab::s_pad;
    tr.Width -= (s_height - Tab::s_dot) / 2 + Tab::s_pad;
    if (m_hovered || m_selected) {
        tr.Width -= s_arrow + s_pad;
    }

    auto ar = r;
    ar.X = r.Width + s_pad / 2;
    ar.Width = s_height;
    
    Graphics g(hdc);
    InitGraphics(g);

    if (m_selected) {
        SolidBrush bgBrush(Style::selectedTab());
        g.FillRectangle(
            &bgBrush,
            toRectF(fullRect)
        );
    }
    if (m_hovered) {
        SolidBrush hoverBrush(Style::hover());
        g.FillRectangle(
            &hoverBrush,
            toRectF(fullRect)
        );
    }

    Graphics gt(hdc);

    InitGraphics(gt);
    Region reg(tr);
    gt.SetClip(&reg);
    StringFormat f;
    f.SetLineAlignment(StringAlignmentCenter);
    f.SetTrimming(StringTrimmingNone);
    f.SetFormatFlags(StringFormatFlagsNoFitBlackBox);
    Font font(hdc, Manager::get()->loadFont(m_font, m_fontSize, m_style));
    SolidBrush brush(color::alpha(m_color, m_hovered || m_selected ? 255 : 125));
    FontFamily family;
    font.GetFamily(&family);
    gt.DrawString(
        m_text.c_str(), -1,
        &font,
        RectF {
            static_cast<float>(tr.X),
            static_cast<float>(tr.Y + 1.5_pxf),
            0.f,
            static_cast<float>(tr.Height)
        },
        &f, &brush
    );
    
    switch (m_type) {
        case Type::Diamond: {
            SolidBrush diamondBrush(
                color::alpha(Style::text(), 80)
            );
            auto dh = static_cast<int>(sqrtf(
                powf(static_cast<float>(Tab::s_dot), 2) * 2.f)
            );
            PointF center(
                r.X + Tab::s_dot / 2.f,
                r.Y + (s_height - dh) / 2.f
            );
            g.TranslateTransform(center.X, center.Y);
            g.RotateTransform(45.f);
            g.FillRectangle(
                &diamondBrush,
                Rect { 0, 0, s_dot, s_dot }
            );
            g.RotateTransform(-45.f);
            g.TranslateTransform(-center.X, -center.Y);
        } break;

        case Type::Plus: {
            Pen pen(color::alpha(Style::text(), 80), m_fontSize / 8.f);
            GraphicsPath path;
            auto pad = (s_height - s_arrow) / 2;
            path.AddLine(
                r.X + Tab::s_dot / 2, r.Y + (s_height - Tab::s_dot) / 2,
                r.X + Tab::s_dot / 2, r.Y + (s_height + Tab::s_dot) / 2
            );
            path.StartFigure();
            path.AddLine(
                r.X, r.Y + s_height / 2,
                r.X + Tab::s_dot, r.Y + s_height / 2
            );
            g.DrawPath(&pen, &path);
        } break;

        default: {
            SolidBrush dotBrush(m_dotColor);
            g.FillEllipse(
                &dotBrush,
                Rect {
                    r.X,
                    r.Y + (s_height - Tab::s_dot) / 2,
                    Tab::s_dot, Tab::s_dot
                }
            );
        } break;
    }

    if (m_arrow && (m_hovered || m_selected) && m_width > s_pad * 2 + s_dot + s_arrow * 2) {
        Pen pen(Style::text(), m_fontSize / 8.f);
        GraphicsPath path;
        auto pad = (s_height - s_arrow) / 2;
        path.AddLine(
            ar.X, ar.Y + pad,
            ar.X + s_arrow / 2, ar.Y + s_height / 2
        );
        path.AddLine(
            ar.X + s_arrow / 2, ar.Y + s_height / 2,
            ar.X, ar.Y + s_height - pad
        );
        g.DrawPath(&pen, &path);
    }

    Widget::paint(hdc, ps);
}

bool Tab::wantsMouse() const {
    return true;
}

HCURSOR Tab::cursor() const {
    return Manager::cursor(IDC_HAND);
}

void Tab::click() {
    if (m_callback) m_callback(this);
    if (!m_button && m_control) {
        m_control->select(this);
    }
}
