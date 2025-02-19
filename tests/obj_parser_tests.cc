#include <gtest/gtest.h>
#include "ObjParser.h"

TEST(ObjParserTest, ParserSkipsUnknownLines){
    ObjParser parser("Hello\nvThis\nfShouldParse\ngNothing", false);

    EXPECT_EQ(parser.groupsInd, 0);
    EXPECT_EQ(parser.vertices.size(), 0);
    EXPECT_EQ(parser.groups.at(0)->getShapes().size(), 0);
}

TEST(ObjParserTest, VertexRecordsCorrectlyParsed){
    ObjParser parser("v -1 1 0\nv -1.0000 0.5000 0.0000\nv 1 0 0\nv 1 1 0", false);

    EXPECT_EQ(parser.groupsInd, 0);
    EXPECT_EQ(parser.vertices.size(), 4);
    EXPECT_TRUE(parser.vertices.at(0).isEqual(Point(-1, 1, 0)));
    EXPECT_TRUE(parser.vertices.at(1).isEqual(Point(-1, 0.5, 0)));
    EXPECT_TRUE(parser.vertices.at(2).isEqual(Point(1, 0, 0)));
    EXPECT_TRUE(parser.vertices.at(3).isEqual(Point(1, 1, 0)));
    EXPECT_EQ(parser.groups.at(0)->getShapes().size(), 0);
}

TEST(ObjParserTest, FaceRecordsCorrectlyParsed){
    ObjParser parser("v -1 1 0\nv -1 0 0\nv 1 0 0\nv 1 1 0\n\nf 1 2 3\nf 1 3 4", false);

    Triangle* t1 = dynamic_cast<Triangle*>(parser.groups.at(0)->getShapes().at(0));
    Triangle* t2 = dynamic_cast<Triangle*>(parser.groups.at(0)->getShapes().at(1));

    EXPECT_EQ(parser.groupsInd, 0);
    EXPECT_EQ(parser.vertices.size(), 4);
    EXPECT_EQ(parser.groups.at(0)->getShapes().size(), 2);
    EXPECT_TRUE(t1->getP1().isEqual(parser.vertices.at(0)));
    EXPECT_TRUE(t1->getP2().isEqual(parser.vertices.at(1)));
    EXPECT_TRUE(t1->getP3().isEqual(parser.vertices.at(2)));
    EXPECT_TRUE(t2->getP1().isEqual(parser.vertices.at(0)));
    EXPECT_TRUE(t2->getP2().isEqual(parser.vertices.at(2)));
    EXPECT_TRUE(t2->getP3().isEqual(parser.vertices.at(3)));
}

TEST(ObjParserTest, FaceRecordsWithMoreThanThreeVertices){
    ObjParser parser("v -1 1 0\nv -1 0 0\nv 1 0 0\nv 1 1 0\nv 0 2 0\n\nf 1 2 3 4 5", false);

    Triangle* t1 = dynamic_cast<Triangle*>(parser.groups.at(0)->getShapes().at(0));
    Triangle* t2 = dynamic_cast<Triangle*>(parser.groups.at(0)->getShapes().at(1));
    Triangle* t3 = dynamic_cast<Triangle*>(parser.groups.at(0)->getShapes().at(2));

    EXPECT_EQ(parser.groupsInd, 0);
    EXPECT_EQ(parser.vertices.size(), 5);
    EXPECT_EQ(parser.groups.at(0)->getShapes().size(), 3);
    EXPECT_TRUE(t1->getP1().isEqual(parser.vertices.at(0)));
    EXPECT_TRUE(t1->getP2().isEqual(parser.vertices.at(1)));
    EXPECT_TRUE(t1->getP3().isEqual(parser.vertices.at(2)));
    EXPECT_TRUE(t2->getP1().isEqual(parser.vertices.at(0)));
    EXPECT_TRUE(t2->getP2().isEqual(parser.vertices.at(2)));
    EXPECT_TRUE(t2->getP3().isEqual(parser.vertices.at(3)));
    EXPECT_TRUE(t3->getP1().isEqual(parser.vertices.at(0)));
    EXPECT_TRUE(t3->getP2().isEqual(parser.vertices.at(3)));
    EXPECT_TRUE(t3->getP3().isEqual(parser.vertices.at(4)));
}

TEST(ObjParserTest, GroupNamesWithCorrectChildrenShapes){
    ObjParser parser("v -1 1 0\nv -1 0 0\nv 1 0 0\nv 1 1 0\n\ng FirstGroup\nf 1 2 3\ng SecondGroup\nf 1 3 4", false);

    Triangle* t1 = dynamic_cast<Triangle*>(parser.groups.at(1)->getShapes().at(0));
    Triangle* t2 = dynamic_cast<Triangle*>(parser.groups.at(2)->getShapes().at(0));

    EXPECT_EQ(parser.vertices.size(), 4);
    EXPECT_EQ(parser.groups.at(0)->getShapes().size(), 0);
    EXPECT_EQ(parser.groups.at(1)->getShapes().size(), 1);
    EXPECT_EQ(parser.groups.at(2)->getShapes().size(), 1);
    EXPECT_EQ(parser.groups.at(1)->name, "FirstGroup");
    EXPECT_EQ(parser.groups.at(2)->name, "SecondGroup");
    EXPECT_TRUE(t1->getP1().isEqual(parser.vertices.at(0)));
    EXPECT_TRUE(t1->getP2().isEqual(parser.vertices.at(1)));
    EXPECT_TRUE(t1->getP3().isEqual(parser.vertices.at(2)));
    EXPECT_TRUE(t2->getP1().isEqual(parser.vertices.at(0)));
    EXPECT_TRUE(t2->getP2().isEqual(parser.vertices.at(2)));
    EXPECT_TRUE(t2->getP3().isEqual(parser.vertices.at(3)));
}

TEST(ObjParser_ObjToGroupTest, GeneratedGroupHasCorrectChildrenGroups){
    ObjParser parser("v -1 1 0\nv -1 0 0\nv 1 0 0\nv 1 1 0\n\ng FirstGroup\nf 1 2 3\ng SecondGroup\nf 1 3 4", false);

    Group* g = parser.objToGroup();
    Group* child1 = dynamic_cast<Group*>(g->getShapes().at(0));
    Group* child2 = dynamic_cast<Group*>(g->getShapes().at(1));

    EXPECT_EQ(g->getShapes().size(), 2);
    EXPECT_EQ(child1->name, "FirstGroup");
    EXPECT_EQ(child2->name, "SecondGroup");
}