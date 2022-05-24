#pragma once

#include <algorithm>

#include "Model.h"

// Проверяют введенные пользователем в форме для авторизации
// данные и, если такой пользователь с соответствующим уровнем
// доступа найден, авторизует пользователя и возвращает true
bool CheckAdminCredentialsAndLogIn(const AuthInfo& form);

bool CheckUserCredentialsAndLogIn(const AuthInfo& form);
// --------
