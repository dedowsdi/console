#ifndef OGRESI_H
#define OGRESI_H

#include "OgreConsolePreRequisite.h"
#include "pacStringInterface.h"
#include "OgreMovableObject.h"

namespace pac {

class OgreSiUtil {
public:
  static MovableSI* createMovableSI(Ogre::MovableObject* mo);
};

class _PacExport _PacExport MovableSI : public StringInterface {
public:
  MovableSI(Ogre::MovableObject* obj);

protected:
  void initParams();

protected:
  Ogre::MovableObject* mMo;
};

class _PacExport LightSI : public MovableSI {
public:
  LightSI(Ogre::Light* light);
  struct _PacExport Visible : public ParamCmd {
    Visible() : ParamCmd("bool") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport LightType : public ParamCmd {
    LightType() : ParamCmd("lightType") {}
    virtual std::string doGet(const void* target) const;
    virtual void doSet(void* target, ArgHandler* handler);
  };

  struct _PacExport Position : public ParamCmd {
    Position() : ParamCmd("real3") {}
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

protected:
  void initParams();

protected:
  static Visible mVisible;
  static LightType msLightType;
  static Position msPosition;
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
  CameraSI(Ogre::Camera* camera);
  mPosition
  mPolygonMode
  mDirection
    

protected:
  void initParams();
};

class _PacExport EntitySI : public MovableSI {
public:
  EntitySI(Ogre::Entity* ent);

protected:
  void initParams();
};

class _PacExport SceneNodeSI : public StringInterface {
public:
  SceneNodeSI(Ogre::SceneNode* sceneNode);

protected:
  Ogre::SceneNode* mSceneNode;
};

class _PacExport SceneSI : public MovableSI {
public:
  SceneSI(Ogre::SceneManager* sceneMgr);

  Ogre::SceneManager* getSceneMgr() const { return mSceneMgr; }
  void setSceneMgr(Ogre::SceneManager* v) { mSceneMgr = v; }

private:
  Ogre::SceneManager* mSceneMgr;
};
}

#endif /* OGRESI_H */
