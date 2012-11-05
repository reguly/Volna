inline void initBathymetry_formula(float *coords, float *values, const float *time) {
  float x = coords[0];
  float y = coords[1];
  float t = *time;
  //insert user formula here
  float val = (0.2216946626f)*(-x)*(x<=((4.0f/5.0f)*(2.0f/0.2216946626f)))+(x>((4.0f/5.0f)*(2.0f/0.2216946626f)))*(x<((6.0f/5.0f)*(2.0f/0.2216946626f)))*((((5.0f/4.0f)*(0.2216946626f)*(0.2216946626f)*x*x)/2.0f)-3.0f*(0.2216946626f)*x+(4.0f/5.0f)*2)-2.0f*(x>=((6.0f/5.0f)*(2.0f/0.2216946626f)))+0.02f*6.0f*(t<((((6.0f/5.0f)-0.45f)*(2.0f/0.2216946626f)+(6.0f/2.0f))/0.6f))*exp(-(4.0f/6.0f)*(4.0f/6.0f)*(x-0.45f*(2.0f/0.2216946626f)-0.6f*t)*(x-0.45f*(2.0f/0.2216946626f)-0.6f*t)-(4.0f/2.0f)*(4.0f/2.0f)*y*y)+0.02f*6.0f*(t>=((((6.0f/5.0f)-0.45f)*(2.0f/0.2216946626f)+(6.0f/2.0f))/0.6))*exp(-(4.0f/6.0f)*(4.0f/6.0f)*(x-(6.0f/5.0f)*(2.0f/0.2216946626f)-(6.0f/2.0f))*(x-(6.0f/5.0f)*(2.0f/0.2216946626f)-(6.0f/2.0f))-(4.0f/2.0f)*(4.0f/2.0f)*y*y);
  //.2*(-5.0-x)*(x<0)-(x>=0)+.2*(t<1)*exp(-(x+3.0-2.0*t)*(x+3.0-2.0*t)-y*y)+.2*(t>=1)*exp(-(x+1.0)*(x+1.0)-y*y);///*...*/0.0;
  values[3] = val;
}
