
#include "SetupWizardAccountBuilder.h"

namespace OCC::Wizard {

SetupWizardAccountBuilder::SetupWizardAccountBuilder()
    : _serverUrl()
    , _workflowType(WorkflowType::UNKNOWN_WORKFLOW)
{
}

bool SetupWizardAccountBuilder::setServerUrl(const QUrl &serverUrl)
{
    if (!serverUrl.isEmpty() && serverUrl.isValid()) {
        _serverUrl = serverUrl;

        // FIXME: this is for debugging purposes
        _workflowType = WorkflowType::HTTP_BASIC_WORKFLOW;

        return true;
    }

    return false;
}

QUrl SetupWizardAccountBuilder::serverUrl() const
{
    return _serverUrl;
}

SetupWizardAccountBuilder::WorkflowType SetupWizardAccountBuilder::workflowType()
{
    return _workflowType;
}

AccountPtr SetupWizardAccountBuilder::build()
{
    // FIXME
    return AccountPtr(nullptr);
}

bool SetupWizardAccountBuilder::setBasicCredentials(const QString &username, const QString &password)
{
    // FIXME: test these credentials
    if (username.isEmpty() || password.isEmpty()) {
        return false;
    }

    _username = username;
    _password = password;
    return true;
}

QString SetupWizardAccountBuilder::username()
{
    return _username;
}

QString SetupWizardAccountBuilder::password()
{
    return _password;
}
}
