#ifndef OGRESI_H
#define OGRESI_H

#include "OgreConsolePreRequisite.h"
#include "pacStringInterface.h"
#include "OgreMovableObject.h"
#include "OgreSiWrapper.h"

namespace pac {

class OgreSiUtil {
public:
  static StringInterface* createMovableSI(Ogre::MovableObject* mo);
};

class _PacExport MovableSI : public StringInterface {
public:
  struct _PacExport Visible : public ParamCmd {
    Visible() : ParamCmd("bool") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };
  struct _PacExport ParentNode : public ParamCmd {
    ParentNode() : ParamCmd("t_sceneNode") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };
  MovableSI(Ogre::MovableObject* obj);
  Ogre::MovableObject* getMovable() const;

protected:
  void buildParams();

protected:
  static Visible msVisible;
  static ParentNode msParentNode;
  Ogre::MovableObject* mMovable;
};

class _PacExport LightSI : public MovableSI {
public:
  struct _PacExport LightType : public ParamCmd {
    LightType() : ParamCmd("en_lightType") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Diffuse : public ParamCmd {
    Diffuse() : ParamCmd("nreal3") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Specular : public ParamCmd {
    Specular() : ParamCmd("nreal3") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Direction : public ParamCmd {
    Direction() : ParamCmd("real3") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport SpotOuter : public ParamCmd {
    SpotOuter() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport SpotInner : public ParamCmd {
    SpotInner() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport SpotFalloff : public ParamCmd {
    SpotFalloff() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport SpotNearClip : public ParamCmd {
    SpotNearClip() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport SpotRange : public ParamCmd {
    SpotRange() : ParamCmd("real3") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Attenuation : public ParamCmd {
    Attenuation() : ParamCmd("real4") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport PowerScale : public ParamCmd {
    PowerScale() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport ShadowFarDist : public ParamCmd {
    ShadowFarDist() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };
  LightSI(Ogre::Light* light);

  Ogre::Light* getLight() const;

protected:
  void buildParams();

protected:
  static LightType msLightType;
  static Diffuse msDiffuse;
  static Specular msSpecular;
  static Direction msDirection;
  static SpotOuter msSpotOuter;
  static SpotInner msSpotInner;
  static SpotFalloff msSpotFalloff;
  static SpotNearClip msSpotNearClip;
  static SpotRange msSpotRange;
  static Attenuation msAttenuation;
  static PowerScale msPowerScale;
  static ShadowFarDist msShadowFarDist;
};

class _PacExport FrustumSI : public MovableSI {
public:
  FrustumSI(Ogre::Frustum* frustum);

  struct _PacExport FOVy : public ParamCmd {
    FOVy() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport AspectRatio : public ParamCmd {
    AspectRatio() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport FocalLength : public ParamCmd {
    FocalLength() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport FrustumOffset : public ParamCmd {
    FrustumOffset() : ParamCmd("real2") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport FrustumExtents : public ParamCmd {
    FrustumExtents() : ParamCmd("real4") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport ProjectionType : public ParamCmd {
    ProjectionType() : ParamCmd("en_projectionType") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport FarClipDistance : public ParamCmd {
    FarClipDistance() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport NearClipDistance : public ParamCmd {
    NearClipDistance() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport ReflectionPlane : public ParamCmd {
    ReflectionPlane() : ParamCmd("readonly") {}
    virtual std::string doGet(const void* target) const;
  };

#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
  struct _PacExport OrientationMode : public ParamCmd {
    OrientationMode() : ParamCmd("en_orientationMode") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };
#endif

  struct _PacExport OrthoWindowWidth : public ParamCmd {
    OrthoWindowWidth() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport OrthoWindowHeight : public ParamCmd {
    OrthoWindowHeight() : ParamCmd("real") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  Ogre::Frustum* getFrustum() const;

protected:
  void buildParams();

protected:
  static FOVy mFOVy;
  static AspectRatio mAspectRatio;
  static FocalLength mFocalLength;
  static FrustumOffset mFrustumOffset;
  static FrustumExtents mFrustumExtents;
  static ProjectionType mProjectionType;
  static FarClipDistance mFarClipDistance;
  static NearClipDistance mNearClipDistance;
  static ReflectionPlane mReflectionPlane;
#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
  static OrientationMode mOrientationMode;
#endif
  static OrthoWindowWidth mOrthoWindowWidth;
  static OrthoWindowHeight mOrthoWindowHeight;
};

class _PacExport CameraSI : public FrustumSI {
public:
  struct _PacExport Position : public ParamCmd {
    Position() : ParamCmd("real3") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  // struct _PacExport PolygonMode : public ParamCmd {
  // PolygonMode() : ParamCmd("en_polygonMode") {}
  // virtual std::string doGet(const void* target) const;
  // virtual void doSet(void* target, ArgHandler* handler);
  //};

  struct _PacExport Direction : public ParamCmd {
    Direction() : ParamCmd("scene_direction") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport LookAt : public ParamCmd {
    LookAt() : ParamCmd("scene_position") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Orientation : public ParamCmd {
    Orientation() : ParamCmd("quaternion") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };


  CameraSI(Ogre::Camera* camera);
  Ogre::Camera* getCamera() const;

protected:
  void buildParams();

protected:
  static Position msPosition;
  static Orientation msOrientation;
  // static PolygonMode msPolygonMode;
  static Direction msDirection;
  static LookAt msLookAt;
};

class _PacExport ItemSI : public MovableSI {
public:
  ItemSI(Ogre::Item* ent);
  Ogre::Item* getItem() const;

protected:
  void buildParams();
};

class _PacExport NodeSI : public StringInterface {
public:
  struct _PacExport Position : public ParamCmd {
    Position() : ParamCmd("real3") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Scale : public ParamCmd {
    Scale() : ParamCmd("real3") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Orientation : public ParamCmd {
    Orientation() : ParamCmd("quaternion") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Parent : public ReadonlyParamCmd {
    Parent() {}
    virtual std::string doGet(const void* target) const;
  };

  struct _PacExport Yaw : public ParamCmd {
    Yaw() : ParamCmd("degree_transform") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };
  struct _PacExport Pitch : public ParamCmd {
    Pitch() : ParamCmd("degree_transform") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };
  struct _PacExport Roll : public ParamCmd {
    Roll() : ParamCmd("degree_transform") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  NodeSI(Ogre::Node* node, const std::string& name = "Node");

  Ogre::Node* getNode() const { return mNode; }

protected:
  void buildParams();

protected:
  Ogre::Node* mNode;
  static Position msPosition;
  static Scale msScale;
  static Orientation msOrientation;
  static Parent msParent;
  static Yaw msYaw;
  static Pitch msPitch;
  static Roll msRoll;
};

class _PacExport SceneNodeSI : public NodeSI {
public:
  struct _PacExport Direction : public ParamCmd {
    Direction() : ParamCmd("sceneNode_direction") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport LookAt : public ParamCmd {
    LookAt() : ParamCmd("sceneNode_lookAt") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  SceneNodeSI(
      Ogre::SceneNode* sceneNode, const std::string& name = "SceneNode");

  Ogre::SceneNode* getSceneNode() const;

protected:
  void buildParams();

  static Direction msDirection;
  static LookAt msLookAt;
};

class _PacExport SceneManagerSI : public StringInterface {
public:
  struct _PacExport ShadowColour : public ParamCmd {
    ShadowColour() : ParamCmd("nreal4") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  // struct _PacExport AmbientLight : public ParamCmd {
  // AmbientLight() : ParamCmd("nreal4") {}
  // virtual std::string doGet(const void* target) const;
  // virtual void doSet(void* target, ArgHandler* handler);
  //};

  struct _PacExport Fog : public ParamCmd {
    Fog() : ParamCmd("fog") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };
  SceneManagerSI(Ogre::SceneManager* sceneMgr);

  Ogre::SceneManager* getSceneMgr() const { return mSceneMgr; }

protected:
  void buildParams();

protected:
  Ogre::SceneManager* mSceneMgr;
  // static ShadowTechnique msShadowTechnique;
  static ShadowColour msShadowColour;
  // static AmbientLight msAmbientLight;
  static Fog msFog;
};
}

#endif /* OGRESI_H */
