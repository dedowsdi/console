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
  void initParams();

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
  void initParams();

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

class _PacExport CameraSI : public MovableSI {
public:
  struct _PacExport Position : public ParamCmd {
    Position() : ParamCmd("real3") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport PolygonMode : public ParamCmd {
    PolygonMode() : ParamCmd("en_polygonMode") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Direction : public ParamCmd {
    Direction() : ParamCmd("real3") {}
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
  void initParams();

protected:
  static Position msPosition;
  static Orientation msOrientation;
  static PolygonMode msPolygonMode;
  static Direction msDirection;
};

class _PacExport EntitySI : public MovableSI {
public:
  EntitySI(Ogre::Entity* ent);
  Ogre::Entity* getEntity() const;

protected:
  void initParams();
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



  NodeSI(Ogre::Node* node);

  Ogre::Node* getNode() const { return mNode; }

protected:
  void initParams();

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

  SceneNodeSI(Ogre::SceneNode* sceneNode);

  Ogre::SceneNode* getSceneNode() const;

protected:
  void initParams();

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

  struct _PacExport AmbientLight : public ParamCmd {
    AmbientLight() : ParamCmd("nreal4") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Fog : public ParamCmd {
    Fog() : ParamCmd("fog") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };
  SceneManagerSI(Ogre::SceneManager* sceneMgr);

  Ogre::SceneManager* getSceneMgr() const { return mSceneMgr; }

protected:
  void initParams();

protected:
  Ogre::SceneManager* mSceneMgr;
  //static ShadowTechnique msShadowTechnique;
  static ShadowColour msShadowColour;
  static AmbientLight msAmbientLight;
  static Fog msFog;
};
}

#endif /* OGRESI_H */
