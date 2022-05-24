#include "Auth.h"

#include "Global.h"

bool CheckAdminCredentialsAndLogIn(const AuthInfo& form)
{
	bool credentials_found = false;
	auto it = std::find_if(users.begin(), users.end(), [form](const UserInfo& user)
		{
			return user.auth_info.login == form.login;
		});
	if (it != users.end())
	{
		if (it->auth_info.password == form.password && it->role == 1)
		{
			credentials_found = true;
			current_user = *it;
		}
	}
	return credentials_found;
}

bool CheckUserCredentialsAndLogIn(const AuthInfo& form)
{
	bool found_credentials = false;
	auto it = std::find_if(users.begin(), users.end(), [form](const UserInfo& user)
		{
			return user.auth_info.login == form.login;
		});
	if (it != users.end())
	{
		if (it->auth_info.password == form.password && it->role == 0)
		{
			found_credentials = true;
			current_user = *it;
		}
	}
	return found_credentials;
}
