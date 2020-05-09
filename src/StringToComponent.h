#pragma once
#include <string>
#include <map>
#include "GameObject.h"
#include "Serializer.h"
class GameObject;
class StringToComponent {
 public:
  template <typename T>
  static void unserializeComponent(GameObject& user,
                                   const SerializedObj& serObj) {
    user.addComponent<T>(serObj);
  }
  static const std::map<std::string,
                        void (*)(GameObject&, const SerializedObj&)>
      unserializeComponentMap;
};