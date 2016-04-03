#ifndef TESTOGREPARTICLESYSTEMSI_HPP
#define TESTOGREPARTICLESYSTEMSI_HPP

#include "pacConsolePreRequisite.h"
#include "testOgreScene.hpp"
#include "pacIntrinsicArgHandler.h"

namespace pac {

TEST_F(TestOgreScene, testParticleSystemSI) {
  ASSERT_TRUE(sgOgreConsole.execute("edmo ParticleSystem " + mParticle0Nameid));
  ASSERT_EQ(sgConsole.getCwd()->getName(), mParticle0Nameid);
  ASSERT_TRUE(sgOgreConsole.execute("get"));
  EXPECT_TRUE(sgOgreConsole.execute("set particle_width 10"));
  EXPECT_TRUE(sgOgreConsole.execute("set particle_height 10"));
  EXPECT_TRUE(sgOgreConsole.execute("set material PE/Lensflare"));
  EXPECT_TRUE(sgOgreConsole.execute("get material"));
  EXPECT_STREQ("material : PE/Lensflare  \n", getLastOutput().c_str());
  ASSERT_TRUE(sgOgreConsole.execute("cd Point_0"));
  ASSERT_TRUE(sgOgreConsole.execute("get"));
  ASSERT_TRUE(sgOgreConsole.execute("cd .." + d + "billboard"));
  ASSERT_TRUE(sgOgreConsole.execute("get"));
  ASSERT_TRUE(sgConsole.execute("adafct " + mParticle0Nameid + " LinearForce "));
  EXPECT_TRUE(sgConsole.execute("get force_application"));
  ASSERT_TRUE(sgConsole.execute("adafct " + mParticle0Nameid + " ColourImage "));
  EXPECT_TRUE(sgConsole.execute("get image"));
}

TEST_F(TestOgreScene, testEmitter) {
  ASSERT_FALSE(
      sgOgreConsole.execute("rmemit " + mParticle0Nameid + " Box " + "0"));
  ASSERT_FALSE(
      sgOgreConsole.execute("rmemit " + mParticle0Nameid + " Point " + "1"));
  ASSERT_TRUE(
      sgOgreConsole.execute("rmemit " + mParticle0Nameid + " Point " + "0"));
  ASSERT_FALSE(
      sgOgreConsole.execute("rmemit " + mParticle0Nameid + " Point " + "0"));

  StringArgHandler* handler =
      static_cast<StringArgHandler*>(sgArgLib.createArgHandler("emitterType"));
  std::for_each(handler->beginStringIter(), handler->endStringIter(),
      [&](const std::string& v) -> void {
        ASSERT_TRUE(
            sgConsole.execute("ademit " + mParticle0Nameid + " " + v + " "));
      });

  std::for_each(handler->beginStringIter(), handler->endStringIter(),
      [&](const std::string& v) -> void {
        ASSERT_TRUE(
            sgConsole.execute("rmemit " + mParticle0Nameid + " " + v + " 0"));
      });
}

TEST_F(TestOgreScene, testAffector) {
  ASSERT_FALSE(
      sgOgreConsole.execute("rmafct " + mParticle0Nameid + " Box " + "0"));
  ASSERT_FALSE(
      sgOgreConsole.execute("rmafct " + mParticle0Nameid + " Point " + "1"));
  ASSERT_FALSE(
      sgOgreConsole.execute("rmafct " + mParticle0Nameid + " Point " + "0"));
  ASSERT_FALSE(
      sgOgreConsole.execute("rmafct " + mParticle0Nameid + " Point " + "0"));

  StringArgHandler* handler =
      static_cast<StringArgHandler*>(sgArgLib.createArgHandler("affectorType"));
  std::for_each(handler->beginStringIter(), handler->endStringIter(),
      [&](const std::string& v) -> void {
        ASSERT_TRUE(
            sgConsole.execute("adafct " + mParticle0Nameid + " " + v + " "));
      });

  std::for_each(handler->beginStringIter(), handler->endStringIter(),
      [&](const std::string& v) -> void {
        ASSERT_TRUE(
            sgConsole.execute("rmafct " + mParticle0Nameid + " " + v + " 0"));
      });
}
}

#endif /* TESTOGREPARTICLESYSTEMSI_HPP */
