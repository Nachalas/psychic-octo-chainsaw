#pragma once

#include <vector>

#include "Model.h"

// ¬ектор, содержащий данные о пользовател€х
extern std::vector<UserInfo> users;
// ¬ектор, содержащий данные о товарах
extern std::vector<ProductEntry> products;
// “екущий авторизованный пользователь
extern UserInfo current_user;
