#include "freeCamera.h"

#include "SEEvent.h"
#include "SESin.h"
#include "SEComTransform.h"

void FreeCameraController::handle(SEEvent &event) {
	switch (event.type) {
	case EVENT_MOUSEMOVE:
		SEComTransform &comTrans = SE_TRANSFORM;

		comTrans[ry] += event.info.mouseMove.delta.x;
		comTrans[rx] += event.info.mouseMove.delta.y;
		comTrans[ty] = sinf(SIN.toRad(comTrans[rx])) * distantceToCenter;

		float xzPlaneDistance = cosf(SIN.toRad(comTrans[rx])) * distantceToCenter;

		comTrans[tx] = cosf(SIN.toRad(comTrans[ry])) * xzPlaneDistance;
		comTrans[tz] = sinf(SIN.toRad(comTrans[ry])) * xzPlaneDistance;
	}
}

void FreeCameraController::onUpdate() {

}