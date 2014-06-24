
// pixel-based dot-light
lowp float wlsign = (gl_FrontFacing) ? 1 : -1;
lowp float worldLight = 0.8 + 0.2 * daylight * dot(wlsign * out_normal.xyz, lightVector.xyz);
