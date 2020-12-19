#include <wx\wx.h>

wxColour wxHSL(double h, double s, double v)
{
	s /= 255;
	v /= 255;

    double hh, p, q, t, ff;
    long i;
    wxColour out;

    if (s <= 0.0) 
    {
        out = wxColour(v, v, v);
        return out;
    }
    hh = h;
    if (hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));

    v *= 255;
    t *= 255;
    p *= 255;
    q *= 255;

    switch (i) 
    {
    case 0:
        out = wxColour(v, t, p);
        break;
    case 1:
        out = wxColour(q, v, p);
        break;
    case 2:
        out = wxColour(p, v, t);
        break;
    case 3:
        out = wxColour(p, q, v);
        break;
    case 4:
        out = wxColour(t, p, v);
        break;
    case 5:
    default:
        out = wxColour(v, p, q);
        break;
    }
    return out;
}