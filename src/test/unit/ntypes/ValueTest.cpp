/* ---------------------------------------------------------------------
 * HTM Community Edition of NuPIC
 * Copyright (C) 2013, Numenta, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero Public License for more details.
 *
 * You should have received a copy of the GNU Affero Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 * --------------------------------------------------------------------- */

/** @file
 * Implementation of Value test
 */

#include <gtest/gtest.h>
#include <htm/ntypes/Value.hpp>

namespace testing { 
    
using namespace htm;

TEST(ValueTest, Scalar) {
  std::shared_ptr<Scalar> s(new Scalar(NTA_BasicType_Int32));
  s->value.int32 = 10;
  Value v(s);
  ASSERT_TRUE(v.isScalar());
  ASSERT_TRUE(!v.isString());
  ASSERT_TRUE(!v.isArray());
  ASSERT_EQ(Value::scalarCategory, v.getCategory());
  ASSERT_EQ(NTA_BasicType_Int32, v.getType());

  std::shared_ptr<Scalar> s1 = v.getScalar();
  ASSERT_TRUE(s1 == s);

  ASSERT_ANY_THROW(v.getArray());
  ASSERT_ANY_THROW(v.getString());

  EXPECT_STREQ("Scalar of type Int32", v.getDescription().c_str());

  Int32 x = v.getScalarT<Int32>();
  ASSERT_EQ(10, x);

  ASSERT_ANY_THROW(v.getScalarT<UInt32>());
}

TEST(ValueTest, Array) {
  std::shared_ptr<Array> s(new Array(NTA_BasicType_Int32));
  s->allocateBuffer(10);
  Value v(s);
  ASSERT_TRUE(v.isArray());
  ASSERT_TRUE(!v.isString());
  ASSERT_TRUE(!v.isScalar());
  ASSERT_EQ(Value::arrayCategory, v.getCategory());
  ASSERT_EQ(NTA_BasicType_Int32, v.getType());

  std::shared_ptr<Array> s1 = v.getArray();
  ASSERT_TRUE(s1 == s);

  ASSERT_ANY_THROW(v.getScalar());
  ASSERT_ANY_THROW(v.getString());
  ASSERT_ANY_THROW(v.getScalarT<Int32>());

  EXPECT_STREQ("Array of type Int32", v.getDescription().c_str());
}

TEST(ValueTest, String) {
  std::string s("hello world");
  Value v(s);
  ASSERT_TRUE(!v.isArray());
  ASSERT_TRUE(v.isString());
  ASSERT_TRUE(!v.isScalar());
  ASSERT_EQ(Value::stringCategory, v.getCategory());
  ASSERT_EQ(NTA_BasicType_Byte, v.getType());

  std::string s1 = v.getString();
  EXPECT_STREQ("hello world", s1.c_str());

  ASSERT_ANY_THROW(v.getScalar());
  ASSERT_ANY_THROW(v.getArray());
  ASSERT_ANY_THROW(v.getScalarT<Int32>());

  EXPECT_STREQ("string (hello world)", v.getDescription().c_str());
}

TEST(ValueTest, ValueMap) {
  std::shared_ptr<Scalar> s(new Scalar(NTA_BasicType_Int32));
  s->value.int32 = 10;
  std::shared_ptr<Array> a(new Array(NTA_BasicType_Real32));
  std::string str("hello world");

  ValueMap vm;
  vm.add("scalar", s);
  vm.add("array", a);
  vm.add("string", str);
  ASSERT_ANY_THROW(vm.add("scalar", s));

  ASSERT_TRUE(vm.contains("scalar"));
  ASSERT_TRUE(vm.contains("array"));
  ASSERT_TRUE(vm.contains("string"));
  ASSERT_TRUE(!vm.contains("foo"));
  ASSERT_TRUE(!vm.contains("scalar2"));
  ASSERT_TRUE(!vm.contains("xscalar"));

  std::shared_ptr<Scalar> s1 = vm.getScalar("scalar");
  ASSERT_TRUE(s1 == s);

  std::shared_ptr<Array> a1 = vm.getArray("array");
  ASSERT_TRUE(a1 == a);

  std::shared_ptr<Scalar> def(new Scalar(NTA_BasicType_Int32));
  Int32 x = vm.getScalarT("scalar", (Int32)20);
  ASSERT_EQ((Int32)10, x);

  x = vm.getScalarT("scalar2", (Int32)20);
  ASSERT_EQ((Int32)20, x);

  Value v = vm.getValue("array");
  ASSERT_EQ(Value::arrayCategory, v.getCategory());
  ASSERT_TRUE(v.getArray() == a);
}
}
