
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
    return AccountPtr(nullptr);
}

}
