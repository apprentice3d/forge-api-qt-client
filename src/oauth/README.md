# Authentication

This SDK comes with an <a
href="https://developer.autodesk.com/en/docs/oauth/v2/overview/"
target="_blank">OAuth 2.0</a> client that allows you to retrieve
2-legged and 3-legged tokens. It also enables you to refresh 3-legged
tokens.
 <!--This tutorial uses both 2-legged and 3-legged tokens for calling
different Data Management endpoints.
-->

## Tutorials
Follow this tutorial to see a step-by-step authentication guide, and
examples of how to use the Forge APIs.

### Create an App

Create an app on the Forge Developer portal. Note the client ID and
client secret.

#### 2-Legged Token

This type of token is given directly to the application. To get a
2-legged token run the following code:

```cpp
#include "oauth/twoleggedapi.h"

...

// create an instance of TwoLeggedApi and set the secrets
Forge::TwoLeggedApi connector;
connector.set_client_id("F5M2IA7AAdenisKgrigormSNQYAFAg5T");
connector.set_client_secret("L603300919812422");


// connect the authenticeateSignal(when TwoLeggedApi instance receives
// a token) with a needed slot or a lambda function as in this example 
QObject::connect(&connector, &Forge::TwoLeggedApi::authenticateSignal,
    [=](Forge::Bearer *result)
{
    qDebug() << "Got token: ";
    qDebug() << *result->get_token_type();
    qDebug() << *result->get_access_token();
    qDebug() << result->get_expiration_time();
    qDebug() << *result->get_refresh_token();

});


// specify the scope of the needed token using the following helper
// function, that will help avoiding type mistakes or space omission
QString scope = Forge::setScopes(Forge::DATA::READ, 
                                 Forge::DATA::WRITE);

// Request a token with mentioned scope
// if not using helper function, you can call directly with
// connector.getTokenWithScope("data:read data:write");
connector.getTokenWithScope(scope);

...

```
In your terminal, you should get a result similar to following one:
```
Got token:
"Bearer"
"Qz8QAdummyrtokenaibNCBi09XFh"
43199
```

For an illustration of integrating ***TwoLeggedApi*** in you application,
 please see the [TwoLeggedTokenUtility](../../samples/TwoLeggedTokenUtility) sample.
