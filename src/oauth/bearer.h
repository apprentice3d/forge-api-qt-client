#pragma once

#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

namespace Forge {

	static QStringList scopes = {
		"data:read",
		"data:write",
		"data:create",
		"data:search",
		"bucket:create",
		"bucket:read",
		"bucket:update",
		"bucket:delete",
		"code:all",
		"account:read",
		"account:write",
		"user-profile:read",
	};

enum class TOKEN_CONTEXT
{
	TWO_LEGGED = 2,
	THREE_LEGGED
};

enum class DATA
{
    READ = 0,
    WRITE,
    CREATE,
    SEARCH,
    LAST
};


enum class BUCKET
{
    CREATE = static_cast<int>(DATA::LAST),
    READ,
    UPDATE,
    DELETE,
    LAST

};

enum class CODE
{
    ALL = static_cast<int>(BUCKET::LAST),
    LAST
};

enum class ACCOUNT
{
    READ = static_cast<int>(CODE::LAST),
    WRITE,
    LAST
};

enum class USER_PROFILE
{
    READ = static_cast<int>(ACCOUNT::LAST),
    LAST
};





class Bearer: public QObject
{
	Q_OBJECT
public:
    Bearer(QString json_data = nullptr, QObject* parent = nullptr);
    ~Bearer() = default;

	void init();

	QString get_token_type() const;
	void set_token_type(const QString& token_type);
	qint32 get_expiration_time() const;
	void set_expiration_time(const qint32 expires_in);
	QString get_access_token() const;
	void set_access_token(const QString& access_token);
	QString get_refresh_token() const;
	void set_refresh_token(const QString& refresh_token);
	TOKEN_CONTEXT get_token_context() const;
	void set_token_context(const TOKEN_CONTEXT token_context);
	QString get_scope() const;
	void set_scope(const QString& char_);

	quint64 get_creation_time() const;
	void set_creation_time(quint64 quint64);

private:
    QString m_token_type;
    qint32 m_expires_in;
    QString m_access_token;
	TOKEN_CONTEXT m_token_context;
    // for 3-legged only
    QString m_refresh_token;

	//auxiliary
	QString m_scope;
	quint64 m_creation_time;


};



/*
 *  AUXILIARY FUNCTIONS
 */
inline QString parseScopeValues(int num, ...)
{
    QString result = "";
    va_list arguments;
    va_start(arguments, num);
    for (int i = 0; i < num; ++i)
    {
        result += scopes.at(va_arg(arguments, int)) + " ";
    }
    va_end(arguments);

    return result;
}

template <class ... Scopes>
QString setScopes(Scopes... arg)
{
    const int n = sizeof...(Scopes);

    return n > 0 ? parseScopeValues(n, arg...) : QString("");
}
}
