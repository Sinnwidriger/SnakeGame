#pragma once

namespace shared
{

  template<typename HandleType>
  class GDIObj
  {
  public:
    GDIObj(HandleType init_obj) :
      obj(init_obj)
    { }
    ~GDIObj()
    {
      DeleteObject(obj);
    }

    operator HandleType() const { return obj; }

  private:
    HandleType obj;
  };

}