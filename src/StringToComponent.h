#pragma once
#include <map>
#include <string>

#include "GameObject.h"
#include "Logging.h"
#include "Serializer.h"
class GameObject;
class StringToComponent {
 public:
  template <typename T>
  static void unserializeComponent(GameObject& user,
                                   const SerializedObj& serObj) {
    LOG(serObj.dump());
    user.addComponent<T>(serObj);
  }
  static const std::map<std::string,
                        void (*)(GameObject&, const SerializedObj&)>
      unserializeComponentMap;
};