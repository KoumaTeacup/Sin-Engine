#include "SEComCollider.h"

#include "SESin.h"
#include "SEComTransform.h"

SEComCollider::SEComCollider(std::string name, std::string tag, SEGameObject *owner) :
	SEComponent(COM_COLLIDER, name, tag, owner) {}

SEComCollider::SEComCollider(const SEComCollider &rhs) :
	SEComponent(rhs),
	colliders(rhs.colliders) {}

SEComCollider& SEComCollider::operator=(const SEComCollider &rhs) {
	SEComponent::operator=(rhs);
	colliders = rhs.colliders;
	return *this;
}

SEVector3f SEComCollider::testWith(const SEComCollider * col) const
{
	SEVector3f selfPos, otherPos, dis, normal, reset = SEVector3f();
	float radius;
	for (auto &i : colliders)
		for (auto &j : col->colliders) {
			selfPos = SE_TRANSFORM.translation() + i.offset;
			otherPos = SIN.getTransform(col).translation() + j.offset;
			dis = otherPos - selfPos;
			switch (i.type) {
			case SECollider::point:
				switch (j.type) {
				case SECollider::box:
					if (j.isPointInBox(selfPos - otherPos,
						SIN.getTransform(col).rotationMatrix())) return otherPos - selfPos;

				case SECollider::sphere: dis *= -1.0f;
					if (dis.lengthSqaure() > j.sqRadius) return SEVector3f(0.0f, 0.0f, 0.0f);
					else return otherPos - selfPos;

				case SECollider::AABB: dis *= -1.0f;
					if (dis.lengthSqaure() > j.sqRadius) return SEVector3f(0.0f, 0.0f, 0.0f);
					if (j.isPointInAABB(dis)) reset += otherPos - selfPos;
					break;

				default: continue;
				}
			case SECollider::plane:
				switch (j.type) {
				case SECollider::box:
					normal = i.shape.normal;
					if (j.isBoxIntersectWithPlane(
						SE_TRANSFORM.rotationMatrix()*SEVector4f(normal[0], normal[1], normal[2]),
						SEVector4f(dis[0], dis[1], dis[2]),
						SIN.getTransform(col).rotationMatrix())) return otherPos - selfPos;
					else return SEVector3f(0.0f, 0.0f, 0.0f);

				case SECollider::sphere:
					normal = i.shape.normal;
					radius = normal * dis;
					if (radius* radius > j.sqRadius) return SEVector3f(0.0f, 0.0f, 0.0f);
					else return otherPos - selfPos;

				case SECollider::AABB:
					normal = i.shape.normal;
					reset += (j.isAABBIntersectWithPlane(normal, dis));
					continue;

				default: continue;
				}
			case SECollider::box: break;
				switch (j.type) {
				case SECollider::point:
					if (i.isPointInBox(otherPos - selfPos, SE_TRANSFORM.rotationMatrix()))
						return otherPos - selfPos;
					else return SEVector3f(0.0f, 0.0f, 0.0f);

				case SECollider::plane:
					normal = j.shape.normal;
					if (i.isBoxIntersectWithPlane(
						SIN.getTransform(col).rotationMatrix()*SEVector4f(normal[0], normal[1], normal[2]),
						SEVector4f(dis[0], dis[1], dis[2]),
						SE_TRANSFORM.rotationMatrix())) return otherPos - selfPos;
					else return SEVector3f(0.0f, 0.0f, 0.0f);

				case SECollider::box: continue;
				case SECollider::sphere: continue;

				default: continue;
				}
			case SECollider::sphere:
				switch (j.type) {
				case SECollider::point: dis *= -1.0f;
					if (dis.lengthSqaure() > i.sqRadius) return SEVector3f(0.0f, 0.0f, 0.0f);
					else return otherPos - selfPos;

				case SECollider::plane:
					normal = j.shape.normal;
					radius = normal * dis;
					if (radius* radius > i.sqRadius) return SEVector3f(0.0f, 0.0f, 0.0f);
					else return otherPos - selfPos;

				case SECollider::box:

				case SECollider::sphere:
					radius = i.shape.radius + j.shape.radius;
					if (dis.lengthSqaure() > radius * radius) return SEVector3f(0.0f, 0.0f, 0.0f);
					else return otherPos - selfPos;

				case SECollider::AABB:
					radius = i.shape.radius + j.shape.radius;
					if (dis.lengthSqaure() > radius * radius) return SEVector3f(0.0f, 0.0f, 0.0f);
					if (i.isAABBInSphere(dis, j.shape.lhw)) return otherPos - selfPos;
					else return SEVector3f(0.0f, 0.0f, 0.0f);

				default: continue;
				}
			case SECollider::AABB:
				switch (j.type) {
				case SECollider::point:
					if (dis.lengthSqaure() > i.sqRadius) return SEVector3f(0.0f, 0.0f, 0.0f);
					if (i.isPointInAABB(dis)) reset += selfPos - otherPos;
					continue;

				case SECollider::plane:
					normal = j.shape.normal;
					reset += (i.isAABBIntersectWithPlane(normal, -dis));
					continue;

				case SECollider::box:
				case SECollider::sphere: dis *= -1.0f;
					radius = i.shape.radius + j.shape.radius;
					if (dis.lengthSqaure() > radius * radius) return SEVector3f(0.0f, 0.0f, 0.0f);
					if (j.isAABBInSphere(dis, i.shape.lhw))return otherPos - selfPos;
					else return SEVector3f(0.0f, 0.0f, 0.0f);

				case SECollider::AABB:
					radius = i.shape.radius + j.shape.radius;
					if (dis.lengthSqaure() > radius * radius) return SEVector3f(0.0f, 0.0f, 0.0f);
					if (i.isAABBInAABB(dis, j.shape.lhw))reset += otherPos - selfPos;
					continue;

				default: continue;
				}
			default: continue;
			}
		}
	return reset;
}

void SEComCollider::onUpdate() {

}

SECollider::SECollider(const SECollider & rhs) :
	type(rhs.type),
	shape(rhs.shape),
	offset(rhs.offset),
	sqRadius(rhs.sqRadius) {}

SECollider & SECollider::calculateSqRadius() {
	switch (type) {
	case box: sqRadius = shape.lhw.lengthSqaure() / 4.0f; break;
	case AABB: sqRadius = shape.lhw.lengthSqaure() / 4.0f; break;
	case sphere: sqRadius = shape.radius * shape.radius; break;
	default: sqRadius = 0.0f; break;
	}
	return *this;
}

bool SECollider::isPointInAABB(SEVector3f point) const {
	point *= 2.0f;
	if (point[0] < -shape.lhw[0]) return false;
	if (point[0] > shape.lhw[0]) return false;
	if (point[1] < -shape.lhw[1]) return false;
	if (point[1] > shape.lhw[1]) return false;
	if (point[2] < -shape.lhw[2]) return false;
	if (point[2] > shape.lhw[2]) return false;
	return true;
}

bool SECollider::isAABBInAABB(SEVector3f dis, SEVector3f lhw) const {
	dis *= 2.0f;
	if (dis[0] + lhw[0] < -shape.lhw[0]) return false;
	if (dis[0] - lhw[0] > shape.lhw[0]) return false;
	if (dis[1] + lhw[1] < -shape.lhw[1]) return false;
	if (dis[1] - lhw[1] > shape.lhw[1]) return false;
	if (dis[2] + lhw[2] < -shape.lhw[2]) return false;
	if (dis[2] - lhw[2] > shape.lhw[2]) return false;
	return true;
}

bool SECollider::isAABBInSphere(SEVector3f dis, SEVector3f lhw) const {
	lhw = lhw / 2.0f;
	SEVector3f normal;
	float length;
	if (abs(dis[0]) < abs(dis[1])) {
		if (abs(dis[0]) < abs(dis[2])) {
			normal = SEVector3f(1.0f, 0.0f, 0.0f);
			length = lhw[0];
		}
		else {
			normal = SEVector3f(0.0f, 0.0f, 1.0f);
			length = lhw[2];
		}
	}
	else if (abs(dis[1]) < abs(dis[2])) {
		normal = SEVector3f(0.0f, 1.0f, 0.0f);
		length = lhw[1];
	}
	else {
		normal = SEVector3f(0.0f, 0.0f, 1.0f);
		length = lhw[2];
	}

	lhw[0] = dis[0] > 0 ? -lhw[0] : lhw[0];
	lhw[1] = dis[1] > 0 ? -lhw[1] : lhw[1];
	lhw[2] = dis[2] > 0 ? -lhw[2] : lhw[2];

	if ((dis + lhw).lengthSqaure() < sqRadius) return true;
	if (abs(dis*normal) < shape.radius) return true;
	return false;
}

bool SECollider::isPointInBox(SEVector3f point, SEMatrix4f rotM) const {
	SEVector3f lhw = shape.lhw / 2.0f;
	// check if within the virtual radius
	if (point.lengthSqaure() > sqRadius) return false;
	// check if between left/right plane
	if (abs(rotM * SEVector4f(1.0f) * SEVector4f(point[0], point[1], point[2])) > lhw[0])
		return false;
	// check if between top/bottom plane
	if (abs(rotM * SEVector4f(0.0f, 1.0f) *	SEVector4f(point[0], point[1], point[2])) > lhw[1])
		return false;
	// check if between front/back plane
	if (abs(rotM * SEVector4f(0.0f, 0.0f, 1.0f) * SEVector4f(point[0], point[1], point[2])) > lhw[2])
		return false;
	return true;
}

SEVector3f SECollider::isAABBIntersectWithPlane(SEVector3f normal, SEVector3f dis) const {
	SEVector3f lhw = shape.lhw / 2.0f;
	float sign = dis*normal;
	if (sign*sign > sqRadius) return SEVector3f(0.0f, 0.0f, 0.0f);
	lhw[0] = dis[0] > 0 ? -lhw[0] : lhw[0];
	lhw[1] = dis[1] > 0 ? -lhw[1] : lhw[1];
	lhw[2] = dis[2] > 0 ? -lhw[2] : lhw[2];
	float closestSign = (dis + lhw) * normal;
	if (closestSign * sign < 0) return -closestSign * normal;
	return SEVector3f(0.0f, 0.0f, 0.0f);
}

bool SECollider::isBoxIntersectWithPlane(SEVector4f normal, SEVector4f dis, SEMatrix4f rotM) const {
	SEVector3f lhw = shape.lhw / 2.0f;
	float sign = dis*normal;
	if (sign*sign > sqRadius) return false;
	if ((dis + rotM * SEVector4f(-lhw[0], -lhw[1], -lhw[2])) * normal * sign < 0) return true;
	if ((dis + rotM * SEVector4f(-lhw[0], -lhw[1], lhw[2])) * normal * sign < 0) return true;
	if ((dis + rotM * SEVector4f(-lhw[0], lhw[1], -lhw[2])) * normal * sign < 0) return true;
	if ((dis + rotM * SEVector4f(-lhw[0], lhw[1], lhw[2])) * normal * sign < 0) return true;
	if ((dis + rotM * SEVector4f(lhw[0], -lhw[1], -lhw[2])) * normal * sign < 0) return true;
	if ((dis + rotM * SEVector4f(lhw[0], -lhw[1], lhw[2])) * normal * sign < 0) return true;
	if ((dis + rotM * SEVector4f(lhw[0], lhw[1], -lhw[2])) * normal * sign < 0) return true;
	if ((dis + rotM * SEVector4f(lhw[0], lhw[1], lhw[2])) * normal * sign < 0) return true;
	return false;
}
