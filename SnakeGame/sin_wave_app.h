#ifndef SNAKE_GAME_APPLICATIONS_SIN_WAVE_APP_H_
#define SNAKE_GAME_APPLICATIONS_SIN_WAVE_APP_H_

#include "window.h"

namespace applications
{

class SinWaveApp : public shared::Window
{
  friend class Window;

private:
  SinWaveApp();

  LRESULT HandleSize(shared::MessageProcParameters mpp);
  LRESULT HandlePaint(shared::MessageProcParameters mpp);
  LRESULT HandleDestroy(shared::MessageProcParameters mpp);

  virtual LRESULT HandleMessage(shared::MessageProcParameters mpp) override;

  std::vector<POINT> points_;
};

}

#endif