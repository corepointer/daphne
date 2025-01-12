#pragma once

class IContext {
  public:
    virtual ~IContext() = default;

    virtual void destroy() = 0;
};
