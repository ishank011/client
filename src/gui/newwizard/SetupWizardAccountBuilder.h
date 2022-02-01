#pragma once

#include <account.h>

namespace OCC::Wizard {
// convenience definition
using AccountPtr = QSharedPointer<Account>;

class SetupWizardAccountBuilder
{
public:
    enum WorkflowType {
        UNKNOWN_WORKFLOW = -1,
        HTTP_BASIC_WORKFLOW = 0,
        OAUTH2_WORKFLOW = 1,
    };

    SetupWizardAccountBuilder();

    /**
     * Set server URL.
     * @param serverUrl URL to server
     * @return true when the URL is valid, false otherwise
     */
    bool setServerUrl(const QUrl &serverUrl);
    QUrl serverUrl() const;

    WorkflowType workflowType();

    /**
     * Attempt to build an account from the previously entered information.
     * @return built account or null if information is still missing
     */
    AccountPtr build();

    bool setBasicCredentials(const QString &username, const QString &password);
    QString username();
    QString password();

private:
    QUrl _serverUrl;

    WorkflowType _workflowType;

    // basic auth workflow data
    QString _username;
    QString _password;
};
}
