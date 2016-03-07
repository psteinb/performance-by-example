---
layout: page
title: Structure without inheritance
minutes: 60
---

> ## Learning Objectives {.objectives}
>
> * generic functions and traits
> * tag dispatching and the boost libraries

Large parts of the code base that Lola is dealing with was written by her predecessor. When she asks her colleaques about this individual they speak highly of him. What she also knows is, that the individual came right out of his studies and just finished a C++ course before he came. 

Lola stumbles over a portion of code that performs some statistical analysis. What she is so surprised about is, that this operation is performed within a derived class, like so:

```
struct statistics_observable {
  
  virtual void update_by(float& _data) = 0;
  virtual void update_by(int& _data) = 0;

  virtual ~statistics_observable(){};

};

struct mean_fp32 : public statistics_observable {

  float		result = 0.;
  std::size_t	size_sample = 0;

  ~mean_fp32(){};
  
  void update_by(float& _data) final override {
    (result += _data);
    size_sample++;
  };

  void update_by(int& _data) final override {
    float casted = float(_data);
    return update_by(casted);
  };

  float value() const {
    return result/size_sample;
  }
  

};

```


> ## Fast code {.callout}
> A key aspect of performant source code is not to waste runtime as much as possible. Thus using templates in runtime critical portions of the code is often considered a must have while being a hallmark of the C++ language at the same time. However, the curious reader is not advised to deduce from this, that templated code must be used whenever possible. C++ offers a lot of ways to structure code, mostly by dynamic (virtual inheritance et al) or static (templates et al) polymorphism. It depends very much on the context when to use what. In case of doubt try to review your code with a collaborator or friend.
