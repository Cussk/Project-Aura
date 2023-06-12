// Copyright Cuss Programming


#include "UI/Widget/AuraUserWidget.h"

//sets widget controller when event called in BP
void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
