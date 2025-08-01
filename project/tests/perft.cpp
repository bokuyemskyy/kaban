#include <gtest/gtest.h>

#include "movegen.hpp"
#include "position.hpp"

TEST(Perft, PerftMicroset) {
    Position pos;
    pos.setFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w");
    EXPECT_EQ(Movegen::perft(pos, 1), 20);
    EXPECT_EQ(Movegen::perft(pos, 2), 400);
    EXPECT_EQ(Movegen::perft(pos, 3), 8902);
}

TEST(Perft, PerftMegaset) {
    Position pos;
    pos.setFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    EXPECT_EQ(Movegen::perft(pos, 4), 197281);
    EXPECT_EQ(Movegen::perft(pos, 6), 119060324);

    pos.setFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 20);
    EXPECT_EQ(Movegen::perft(pos, 2), 400);
    EXPECT_EQ(Movegen::perft(pos, 3), 8902);
    EXPECT_EQ(Movegen::perft(pos, 4), 197281);
    EXPECT_EQ(Movegen::perft(pos, 5), 4865609);
    EXPECT_EQ(Movegen::perft(pos, 6), 119060324);

    pos.setFromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 48);
    EXPECT_EQ(Movegen::perft(pos, 2), 2039);
    EXPECT_EQ(Movegen::perft(pos, 3), 97862);
    EXPECT_EQ(Movegen::perft(pos, 4), 4085603);
    EXPECT_EQ(Movegen::perft(pos, 5), 193690690);

    pos.setFromFEN("4k3/8/8/8/8/8/8/4K2R w K - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 15);
    EXPECT_EQ(Movegen::perft(pos, 2), 66);
    EXPECT_EQ(Movegen::perft(pos, 3), 1197);
    EXPECT_EQ(Movegen::perft(pos, 4), 7059);
    EXPECT_EQ(Movegen::perft(pos, 5), 133987);
    EXPECT_EQ(Movegen::perft(pos, 6), 764643);

    pos.setFromFEN("4k3/8/8/8/8/8/8/R3K3 w Q - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 16);
    EXPECT_EQ(Movegen::perft(pos, 2), 71);
    EXPECT_EQ(Movegen::perft(pos, 3), 1287);
    EXPECT_EQ(Movegen::perft(pos, 4), 7626);
    EXPECT_EQ(Movegen::perft(pos, 5), 145232);
    EXPECT_EQ(Movegen::perft(pos, 6), 846648);

    pos.setFromFEN("4k2r/8/8/8/8/8/8/4K3 w k - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 75);
    EXPECT_EQ(Movegen::perft(pos, 3), 459);
    EXPECT_EQ(Movegen::perft(pos, 4), 8290);
    EXPECT_EQ(Movegen::perft(pos, 5), 47635);
    EXPECT_EQ(Movegen::perft(pos, 6), 899442);

    pos.setFromFEN("r3k3/8/8/8/8/8/8/4K3 w q - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 80);
    EXPECT_EQ(Movegen::perft(pos, 3), 493);
    EXPECT_EQ(Movegen::perft(pos, 4), 8897);
    EXPECT_EQ(Movegen::perft(pos, 5), 52710);
    EXPECT_EQ(Movegen::perft(pos, 6), 1001523);

    pos.setFromFEN("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 26);
    EXPECT_EQ(Movegen::perft(pos, 2), 112);
    EXPECT_EQ(Movegen::perft(pos, 3), 3189);
    EXPECT_EQ(Movegen::perft(pos, 4), 17945);
    EXPECT_EQ(Movegen::perft(pos, 5), 532933);
    EXPECT_EQ(Movegen::perft(pos, 6), 2788982);

    pos.setFromFEN("r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 130);
    EXPECT_EQ(Movegen::perft(pos, 3), 782);
    EXPECT_EQ(Movegen::perft(pos, 4), 22180);
    EXPECT_EQ(Movegen::perft(pos, 5), 118882);
    EXPECT_EQ(Movegen::perft(pos, 6), 3517770);

    pos.setFromFEN("8/8/8/8/8/8/6k1/4K2R w K - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 12);
    EXPECT_EQ(Movegen::perft(pos, 2), 38);
    EXPECT_EQ(Movegen::perft(pos, 3), 564);
    EXPECT_EQ(Movegen::perft(pos, 4), 2219);
    EXPECT_EQ(Movegen::perft(pos, 5), 37735);
    EXPECT_EQ(Movegen::perft(pos, 6), 185867);

    pos.setFromFEN("8/8/8/8/8/8/1k6/R3K3 w Q - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 15);
    EXPECT_EQ(Movegen::perft(pos, 2), 65);
    EXPECT_EQ(Movegen::perft(pos, 3), 1018);
    EXPECT_EQ(Movegen::perft(pos, 4), 4573);
    EXPECT_EQ(Movegen::perft(pos, 5), 80619);
    EXPECT_EQ(Movegen::perft(pos, 6), 413018);

    pos.setFromFEN("4k2r/6K1/8/8/8/8/8/8 w k - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 32);
    EXPECT_EQ(Movegen::perft(pos, 3), 134);
    EXPECT_EQ(Movegen::perft(pos, 4), 2073);
    EXPECT_EQ(Movegen::perft(pos, 5), 10485);
    EXPECT_EQ(Movegen::perft(pos, 6), 179869);

    pos.setFromFEN("r3k3/1K6/8/8/8/8/8/8 w q - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 49);
    EXPECT_EQ(Movegen::perft(pos, 3), 243);
    EXPECT_EQ(Movegen::perft(pos, 4), 3991);
    EXPECT_EQ(Movegen::perft(pos, 5), 20780);
    EXPECT_EQ(Movegen::perft(pos, 6), 367724);

    pos.setFromFEN("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 26);
    EXPECT_EQ(Movegen::perft(pos, 2), 568);
    EXPECT_EQ(Movegen::perft(pos, 3), 13744);
    EXPECT_EQ(Movegen::perft(pos, 4), 314346);
    EXPECT_EQ(Movegen::perft(pos, 5), 7594526);
    EXPECT_EQ(Movegen::perft(pos, 6), 179862938);

    pos.setFromFEN("r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 25);
    EXPECT_EQ(Movegen::perft(pos, 2), 567);
    EXPECT_EQ(Movegen::perft(pos, 3), 14095);
    EXPECT_EQ(Movegen::perft(pos, 4), 328965);
    EXPECT_EQ(Movegen::perft(pos, 5), 8153719);
    EXPECT_EQ(Movegen::perft(pos, 6), 195629489);

    pos.setFromFEN("r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 25);
    EXPECT_EQ(Movegen::perft(pos, 2), 548);
    EXPECT_EQ(Movegen::perft(pos, 3), 13502);
    EXPECT_EQ(Movegen::perft(pos, 4), 312835);
    EXPECT_EQ(Movegen::perft(pos, 5), 7736373);
    EXPECT_EQ(Movegen::perft(pos, 6), 184411439);

    pos.setFromFEN("r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 25);
    EXPECT_EQ(Movegen::perft(pos, 2), 547);
    EXPECT_EQ(Movegen::perft(pos, 3), 13579);
    EXPECT_EQ(Movegen::perft(pos, 4), 316214);
    EXPECT_EQ(Movegen::perft(pos, 5), 7878456);
    EXPECT_EQ(Movegen::perft(pos, 6), 189224276);

    pos.setFromFEN("1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 26);
    EXPECT_EQ(Movegen::perft(pos, 2), 583);
    EXPECT_EQ(Movegen::perft(pos, 3), 14252);
    EXPECT_EQ(Movegen::perft(pos, 4), 334705);
    EXPECT_EQ(Movegen::perft(pos, 5), 8198901);
    EXPECT_EQ(Movegen::perft(pos, 6), 198328929);

    pos.setFromFEN("2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 25);
    EXPECT_EQ(Movegen::perft(pos, 2), 560);
    EXPECT_EQ(Movegen::perft(pos, 3), 13592);
    EXPECT_EQ(Movegen::perft(pos, 4), 317324);
    EXPECT_EQ(Movegen::perft(pos, 5), 7710115);
    EXPECT_EQ(Movegen::perft(pos, 6), 185959088);

    pos.setFromFEN("r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 25);
    EXPECT_EQ(Movegen::perft(pos, 2), 560);
    EXPECT_EQ(Movegen::perft(pos, 3), 13607);
    EXPECT_EQ(Movegen::perft(pos, 4), 320792);
    EXPECT_EQ(Movegen::perft(pos, 5), 7848606);
    EXPECT_EQ(Movegen::perft(pos, 6), 190755813);

    pos.setFromFEN("4k3/8/8/8/8/8/8/4K2R b K - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 75);
    EXPECT_EQ(Movegen::perft(pos, 3), 459);
    EXPECT_EQ(Movegen::perft(pos, 4), 8290);
    EXPECT_EQ(Movegen::perft(pos, 5), 47635);
    EXPECT_EQ(Movegen::perft(pos, 6), 899442);

    pos.setFromFEN("4k3/8/8/8/8/8/8/R3K3 b Q - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 80);
    EXPECT_EQ(Movegen::perft(pos, 3), 493);
    EXPECT_EQ(Movegen::perft(pos, 4), 8897);
    EXPECT_EQ(Movegen::perft(pos, 5), 52710);
    EXPECT_EQ(Movegen::perft(pos, 6), 1001523);

    pos.setFromFEN("4k2r/8/8/8/8/8/8/4K3 b k - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 15);
    EXPECT_EQ(Movegen::perft(pos, 2), 66);
    EXPECT_EQ(Movegen::perft(pos, 3), 1197);
    EXPECT_EQ(Movegen::perft(pos, 4), 7059);
    EXPECT_EQ(Movegen::perft(pos, 5), 133987);
    EXPECT_EQ(Movegen::perft(pos, 6), 764643);

    pos.setFromFEN("r3k3/8/8/8/8/8/8/4K3 b q - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 16);
    EXPECT_EQ(Movegen::perft(pos, 2), 71);
    EXPECT_EQ(Movegen::perft(pos, 3), 1287);
    EXPECT_EQ(Movegen::perft(pos, 4), 7626);
    EXPECT_EQ(Movegen::perft(pos, 5), 145232);
    EXPECT_EQ(Movegen::perft(pos, 6), 846648);

    pos.setFromFEN("4k3/8/8/8/8/8/8/R3K2R b KQ - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 130);
    EXPECT_EQ(Movegen::perft(pos, 3), 782);
    EXPECT_EQ(Movegen::perft(pos, 4), 22180);
    EXPECT_EQ(Movegen::perft(pos, 5), 118882);
    EXPECT_EQ(Movegen::perft(pos, 6), 3517770);

    pos.setFromFEN("r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 26);
    EXPECT_EQ(Movegen::perft(pos, 2), 112);
    EXPECT_EQ(Movegen::perft(pos, 3), 3189);
    EXPECT_EQ(Movegen::perft(pos, 4), 17945);
    EXPECT_EQ(Movegen::perft(pos, 5), 532933);
    EXPECT_EQ(Movegen::perft(pos, 6), 2788982);

    pos.setFromFEN("8/8/8/8/8/8/6k1/4K2R b K - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 32);
    EXPECT_EQ(Movegen::perft(pos, 3), 134);
    EXPECT_EQ(Movegen::perft(pos, 4), 2073);
    EXPECT_EQ(Movegen::perft(pos, 5), 10485);
    EXPECT_EQ(Movegen::perft(pos, 6), 179869);

    pos.setFromFEN("8/8/8/8/8/8/1k6/R3K3 b Q - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 49);
    EXPECT_EQ(Movegen::perft(pos, 3), 243);
    EXPECT_EQ(Movegen::perft(pos, 4), 3991);
    EXPECT_EQ(Movegen::perft(pos, 5), 20780);
    EXPECT_EQ(Movegen::perft(pos, 6), 367724);

    pos.setFromFEN("4k2r/6K1/8/8/8/8/8/8 b k - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 12);
    EXPECT_EQ(Movegen::perft(pos, 2), 38);
    EXPECT_EQ(Movegen::perft(pos, 3), 564);
    EXPECT_EQ(Movegen::perft(pos, 4), 2219);
    EXPECT_EQ(Movegen::perft(pos, 5), 37735);
    EXPECT_EQ(Movegen::perft(pos, 6), 185867);

    pos.setFromFEN("r3k3/1K6/8/8/8/8/8/8 b q - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 15);
    EXPECT_EQ(Movegen::perft(pos, 2), 65);
    EXPECT_EQ(Movegen::perft(pos, 3), 1018);
    EXPECT_EQ(Movegen::perft(pos, 4), 4573);
    EXPECT_EQ(Movegen::perft(pos, 5), 80619);
    EXPECT_EQ(Movegen::perft(pos, 6), 413018);

    pos.setFromFEN("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 26);
    EXPECT_EQ(Movegen::perft(pos, 2), 568);
    EXPECT_EQ(Movegen::perft(pos, 3), 13744);
    EXPECT_EQ(Movegen::perft(pos, 4), 314346);
    EXPECT_EQ(Movegen::perft(pos, 5), 7594526);
    EXPECT_EQ(Movegen::perft(pos, 6), 179862938);

    pos.setFromFEN("r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 26);
    EXPECT_EQ(Movegen::perft(pos, 2), 583);
    EXPECT_EQ(Movegen::perft(pos, 3), 14252);
    EXPECT_EQ(Movegen::perft(pos, 4), 334705);
    EXPECT_EQ(Movegen::perft(pos, 5), 8198901);
    EXPECT_EQ(Movegen::perft(pos, 6), 198328929);

    pos.setFromFEN("r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 25);
    EXPECT_EQ(Movegen::perft(pos, 2), 560);
    EXPECT_EQ(Movegen::perft(pos, 3), 13592);
    EXPECT_EQ(Movegen::perft(pos, 4), 317324);
    EXPECT_EQ(Movegen::perft(pos, 5), 7710115);
    EXPECT_EQ(Movegen::perft(pos, 6), 185959088);

    pos.setFromFEN("r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 25);
    EXPECT_EQ(Movegen::perft(pos, 2), 560);
    EXPECT_EQ(Movegen::perft(pos, 3), 13607);
    EXPECT_EQ(Movegen::perft(pos, 4), 320792);
    EXPECT_EQ(Movegen::perft(pos, 5), 7848606);
    EXPECT_EQ(Movegen::perft(pos, 6), 190755813);

    pos.setFromFEN("1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 25);
    EXPECT_EQ(Movegen::perft(pos, 2), 567);
    EXPECT_EQ(Movegen::perft(pos, 3), 14095);
    EXPECT_EQ(Movegen::perft(pos, 4), 328965);
    EXPECT_EQ(Movegen::perft(pos, 5), 8153719);
    EXPECT_EQ(Movegen::perft(pos, 6), 195629489);

    pos.setFromFEN("2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 25);
    EXPECT_EQ(Movegen::perft(pos, 2), 548);
    EXPECT_EQ(Movegen::perft(pos, 3), 13502);
    EXPECT_EQ(Movegen::perft(pos, 4), 312835);
    EXPECT_EQ(Movegen::perft(pos, 5), 7736373);
    EXPECT_EQ(Movegen::perft(pos, 6), 184411439);

    pos.setFromFEN("r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 25);
    EXPECT_EQ(Movegen::perft(pos, 2), 547);
    EXPECT_EQ(Movegen::perft(pos, 3), 13579);
    EXPECT_EQ(Movegen::perft(pos, 4), 316214);
    EXPECT_EQ(Movegen::perft(pos, 5), 7878456);
    EXPECT_EQ(Movegen::perft(pos, 6), 189224276);

    pos.setFromFEN("8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 14);
    EXPECT_EQ(Movegen::perft(pos, 2), 195);
    EXPECT_EQ(Movegen::perft(pos, 3), 2760);
    EXPECT_EQ(Movegen::perft(pos, 4), 38675);
    EXPECT_EQ(Movegen::perft(pos, 5), 570726);
    EXPECT_EQ(Movegen::perft(pos, 6), 8107539);

    pos.setFromFEN("8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 11);
    EXPECT_EQ(Movegen::perft(pos, 2), 156);
    EXPECT_EQ(Movegen::perft(pos, 3), 1636);
    EXPECT_EQ(Movegen::perft(pos, 4), 20534);
    EXPECT_EQ(Movegen::perft(pos, 5), 223507);
    EXPECT_EQ(Movegen::perft(pos, 6), 2594412);

    pos.setFromFEN("8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 19);
    EXPECT_EQ(Movegen::perft(pos, 2), 289);
    EXPECT_EQ(Movegen::perft(pos, 3), 4442);
    EXPECT_EQ(Movegen::perft(pos, 4), 73584);
    EXPECT_EQ(Movegen::perft(pos, 5), 1198299);
    EXPECT_EQ(Movegen::perft(pos, 6), 19870403);

    pos.setFromFEN("K7/8/2n5/1n6/8/8/8/k6N w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 51);
    EXPECT_EQ(Movegen::perft(pos, 3), 345);
    EXPECT_EQ(Movegen::perft(pos, 4), 5301);
    EXPECT_EQ(Movegen::perft(pos, 5), 38348);
    EXPECT_EQ(Movegen::perft(pos, 6), 588695);

    pos.setFromFEN("k7/8/2N5/1N6/8/8/8/K6n w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 17);
    EXPECT_EQ(Movegen::perft(pos, 2), 54);
    EXPECT_EQ(Movegen::perft(pos, 3), 835);
    EXPECT_EQ(Movegen::perft(pos, 4), 5910);
    EXPECT_EQ(Movegen::perft(pos, 5), 92250);
    EXPECT_EQ(Movegen::perft(pos, 6), 688780);

    pos.setFromFEN("8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 15);
    EXPECT_EQ(Movegen::perft(pos, 2), 193);
    EXPECT_EQ(Movegen::perft(pos, 3), 2816);
    EXPECT_EQ(Movegen::perft(pos, 4), 40039);
    EXPECT_EQ(Movegen::perft(pos, 5), 582642);
    EXPECT_EQ(Movegen::perft(pos, 6), 8503277);

    pos.setFromFEN("8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 16);
    EXPECT_EQ(Movegen::perft(pos, 2), 180);
    EXPECT_EQ(Movegen::perft(pos, 3), 2290);
    EXPECT_EQ(Movegen::perft(pos, 4), 24640);
    EXPECT_EQ(Movegen::perft(pos, 5), 288141);
    EXPECT_EQ(Movegen::perft(pos, 6), 3147566);

    pos.setFromFEN("8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 68);
    EXPECT_EQ(Movegen::perft(pos, 3), 1118);
    EXPECT_EQ(Movegen::perft(pos, 4), 16199);
    EXPECT_EQ(Movegen::perft(pos, 5), 281190);
    EXPECT_EQ(Movegen::perft(pos, 6), 4405103);

    pos.setFromFEN("K7/8/2n5/1n6/8/8/8/k6N b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 17);
    EXPECT_EQ(Movegen::perft(pos, 2), 54);
    EXPECT_EQ(Movegen::perft(pos, 3), 835);
    EXPECT_EQ(Movegen::perft(pos, 4), 5910);
    EXPECT_EQ(Movegen::perft(pos, 5), 92250);
    EXPECT_EQ(Movegen::perft(pos, 6), 688780);

    pos.setFromFEN("k7/8/2N5/1N6/8/8/8/K6n b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 51);
    EXPECT_EQ(Movegen::perft(pos, 3), 345);
    EXPECT_EQ(Movegen::perft(pos, 4), 5301);
    EXPECT_EQ(Movegen::perft(pos, 5), 38348);
    EXPECT_EQ(Movegen::perft(pos, 6), 588695);

    pos.setFromFEN("B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 17);
    EXPECT_EQ(Movegen::perft(pos, 2), 278);
    EXPECT_EQ(Movegen::perft(pos, 3), 4607);
    EXPECT_EQ(Movegen::perft(pos, 4), 76778);
    EXPECT_EQ(Movegen::perft(pos, 5), 1320507);
    EXPECT_EQ(Movegen::perft(pos, 6), 22823890);

    pos.setFromFEN("8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 21);
    EXPECT_EQ(Movegen::perft(pos, 2), 316);
    EXPECT_EQ(Movegen::perft(pos, 3), 5744);
    EXPECT_EQ(Movegen::perft(pos, 4), 93338);
    EXPECT_EQ(Movegen::perft(pos, 5), 1713368);
    EXPECT_EQ(Movegen::perft(pos, 6), 28861171);

    pos.setFromFEN("k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 21);
    EXPECT_EQ(Movegen::perft(pos, 2), 144);
    EXPECT_EQ(Movegen::perft(pos, 3), 3242);
    EXPECT_EQ(Movegen::perft(pos, 4), 32955);
    EXPECT_EQ(Movegen::perft(pos, 5), 787524);
    EXPECT_EQ(Movegen::perft(pos, 6), 7881673);

    pos.setFromFEN("K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 7);
    EXPECT_EQ(Movegen::perft(pos, 2), 143);
    EXPECT_EQ(Movegen::perft(pos, 3), 1416);
    EXPECT_EQ(Movegen::perft(pos, 4), 31787);
    EXPECT_EQ(Movegen::perft(pos, 5), 310862);
    EXPECT_EQ(Movegen::perft(pos, 6), 7382896);

    pos.setFromFEN("B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 6);
    EXPECT_EQ(Movegen::perft(pos, 2), 106);
    EXPECT_EQ(Movegen::perft(pos, 3), 1829);
    EXPECT_EQ(Movegen::perft(pos, 4), 31151);
    EXPECT_EQ(Movegen::perft(pos, 5), 530585);
    EXPECT_EQ(Movegen::perft(pos, 6), 9250746);

    pos.setFromFEN("8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 17);
    EXPECT_EQ(Movegen::perft(pos, 2), 309);
    EXPECT_EQ(Movegen::perft(pos, 3), 5133);
    EXPECT_EQ(Movegen::perft(pos, 4), 93603);
    EXPECT_EQ(Movegen::perft(pos, 5), 1591064);
    EXPECT_EQ(Movegen::perft(pos, 6), 29027891);

    pos.setFromFEN("k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 7);
    EXPECT_EQ(Movegen::perft(pos, 2), 143);
    EXPECT_EQ(Movegen::perft(pos, 3), 1416);
    EXPECT_EQ(Movegen::perft(pos, 4), 31787);
    EXPECT_EQ(Movegen::perft(pos, 5), 310862);
    EXPECT_EQ(Movegen::perft(pos, 6), 7382896);

    pos.setFromFEN("K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 21);
    EXPECT_EQ(Movegen::perft(pos, 2), 144);
    EXPECT_EQ(Movegen::perft(pos, 3), 3242);
    EXPECT_EQ(Movegen::perft(pos, 4), 32955);
    EXPECT_EQ(Movegen::perft(pos, 5), 787524);
    EXPECT_EQ(Movegen::perft(pos, 6), 7881673);

    pos.setFromFEN("7k/RR6/8/8/8/8/rr6/7K w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 19);
    EXPECT_EQ(Movegen::perft(pos, 2), 275);
    EXPECT_EQ(Movegen::perft(pos, 3), 5300);
    EXPECT_EQ(Movegen::perft(pos, 4), 104342);
    EXPECT_EQ(Movegen::perft(pos, 5), 2161211);
    EXPECT_EQ(Movegen::perft(pos, 6), 44956585);

    pos.setFromFEN("R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 36);
    EXPECT_EQ(Movegen::perft(pos, 2), 1027);
    EXPECT_EQ(Movegen::perft(pos, 3), 29215);
    EXPECT_EQ(Movegen::perft(pos, 4), 771461);
    EXPECT_EQ(Movegen::perft(pos, 5), 20506480);
    EXPECT_EQ(Movegen::perft(pos, 6), 525169084);

    pos.setFromFEN("7k/RR6/8/8/8/8/rr6/7K b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 19);
    EXPECT_EQ(Movegen::perft(pos, 2), 275);
    EXPECT_EQ(Movegen::perft(pos, 3), 5300);
    EXPECT_EQ(Movegen::perft(pos, 4), 104342);
    EXPECT_EQ(Movegen::perft(pos, 5), 2161211);
    EXPECT_EQ(Movegen::perft(pos, 6), 44956585);

    pos.setFromFEN("R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 36);
    EXPECT_EQ(Movegen::perft(pos, 2), 1027);
    EXPECT_EQ(Movegen::perft(pos, 3), 29227);
    EXPECT_EQ(Movegen::perft(pos, 4), 771368);
    EXPECT_EQ(Movegen::perft(pos, 5), 20521342);
    EXPECT_EQ(Movegen::perft(pos, 6), 524966748);

    pos.setFromFEN("6kq/8/8/8/8/8/8/7K w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 2);
    EXPECT_EQ(Movegen::perft(pos, 2), 36);
    EXPECT_EQ(Movegen::perft(pos, 3), 143);
    EXPECT_EQ(Movegen::perft(pos, 4), 3637);
    EXPECT_EQ(Movegen::perft(pos, 5), 14893);
    EXPECT_EQ(Movegen::perft(pos, 6), 391507);

    pos.setFromFEN("6KQ/8/8/8/8/8/8/7k b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 2);
    EXPECT_EQ(Movegen::perft(pos, 2), 36);
    EXPECT_EQ(Movegen::perft(pos, 3), 143);
    EXPECT_EQ(Movegen::perft(pos, 4), 3637);
    EXPECT_EQ(Movegen::perft(pos, 5), 14893);
    EXPECT_EQ(Movegen::perft(pos, 6), 391507);

    pos.setFromFEN("K7/8/8/3Q4/4q3/8/8/7k w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 6);
    EXPECT_EQ(Movegen::perft(pos, 2), 35);
    EXPECT_EQ(Movegen::perft(pos, 3), 495);
    EXPECT_EQ(Movegen::perft(pos, 4), 8349);
    EXPECT_EQ(Movegen::perft(pos, 5), 166741);
    EXPECT_EQ(Movegen::perft(pos, 6), 3370175);

    pos.setFromFEN("6qk/8/8/8/8/8/8/7K b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 22);
    EXPECT_EQ(Movegen::perft(pos, 2), 43);
    EXPECT_EQ(Movegen::perft(pos, 3), 1015);
    EXPECT_EQ(Movegen::perft(pos, 4), 4167);
    EXPECT_EQ(Movegen::perft(pos, 5), 105749);
    EXPECT_EQ(Movegen::perft(pos, 6), 419369);

    pos.setFromFEN("6KQ/8/8/8/8/8/8/7k b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 2);
    EXPECT_EQ(Movegen::perft(pos, 2), 36);
    EXPECT_EQ(Movegen::perft(pos, 3), 143);
    EXPECT_EQ(Movegen::perft(pos, 4), 3637);
    EXPECT_EQ(Movegen::perft(pos, 5), 14893);
    EXPECT_EQ(Movegen::perft(pos, 6), 391507);

    pos.setFromFEN("K7/8/8/3Q4/4q3/8/8/7k b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 6);
    EXPECT_EQ(Movegen::perft(pos, 2), 35);
    EXPECT_EQ(Movegen::perft(pos, 3), 495);
    EXPECT_EQ(Movegen::perft(pos, 4), 8349);
    EXPECT_EQ(Movegen::perft(pos, 5), 166741);
    EXPECT_EQ(Movegen::perft(pos, 6), 3370175);

    pos.setFromFEN("8/8/8/8/8/K7/P7/k7 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 7);
    EXPECT_EQ(Movegen::perft(pos, 3), 43);
    EXPECT_EQ(Movegen::perft(pos, 4), 199);
    EXPECT_EQ(Movegen::perft(pos, 5), 1347);
    EXPECT_EQ(Movegen::perft(pos, 6), 6249);

    pos.setFromFEN("8/8/8/8/8/7K/7P/7k w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 7);
    EXPECT_EQ(Movegen::perft(pos, 3), 43);
    EXPECT_EQ(Movegen::perft(pos, 4), 199);
    EXPECT_EQ(Movegen::perft(pos, 5), 1347);
    EXPECT_EQ(Movegen::perft(pos, 6), 6249);

    pos.setFromFEN("K7/p7/k7/8/8/8/8/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 1);
    EXPECT_EQ(Movegen::perft(pos, 2), 3);
    EXPECT_EQ(Movegen::perft(pos, 3), 12);
    EXPECT_EQ(Movegen::perft(pos, 4), 80);
    EXPECT_EQ(Movegen::perft(pos, 5), 342);
    EXPECT_EQ(Movegen::perft(pos, 6), 2343);

    pos.setFromFEN("7K/7p/7k/8/8/8/8/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 1);
    EXPECT_EQ(Movegen::perft(pos, 2), 3);
    EXPECT_EQ(Movegen::perft(pos, 3), 12);
    EXPECT_EQ(Movegen::perft(pos, 4), 80);
    EXPECT_EQ(Movegen::perft(pos, 5), 342);
    EXPECT_EQ(Movegen::perft(pos, 6), 2343);

    pos.setFromFEN("8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 7);
    EXPECT_EQ(Movegen::perft(pos, 2), 35);
    EXPECT_EQ(Movegen::perft(pos, 3), 210);
    EXPECT_EQ(Movegen::perft(pos, 4), 1091);
    EXPECT_EQ(Movegen::perft(pos, 5), 7028);
    EXPECT_EQ(Movegen::perft(pos, 6), 34834);

    pos.setFromFEN("8/8/8/8/8/K7/P7/k7 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 1);
    EXPECT_EQ(Movegen::perft(pos, 2), 3);
    EXPECT_EQ(Movegen::perft(pos, 3), 12);
    EXPECT_EQ(Movegen::perft(pos, 4), 80);
    EXPECT_EQ(Movegen::perft(pos, 5), 342);
    EXPECT_EQ(Movegen::perft(pos, 6), 2343);

    pos.setFromFEN("8/8/8/8/8/7K/7P/7k b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 1);
    EXPECT_EQ(Movegen::perft(pos, 2), 3);
    EXPECT_EQ(Movegen::perft(pos, 3), 12);
    EXPECT_EQ(Movegen::perft(pos, 4), 80);
    EXPECT_EQ(Movegen::perft(pos, 5), 342);
    EXPECT_EQ(Movegen::perft(pos, 6), 2343);

    pos.setFromFEN("K7/p7/k7/8/8/8/8/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 7);
    EXPECT_EQ(Movegen::perft(pos, 3), 43);
    EXPECT_EQ(Movegen::perft(pos, 4), 199);
    EXPECT_EQ(Movegen::perft(pos, 5), 1347);
    EXPECT_EQ(Movegen::perft(pos, 6), 6249);

    pos.setFromFEN("7K/7p/7k/8/8/8/8/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 7);
    EXPECT_EQ(Movegen::perft(pos, 3), 43);
    EXPECT_EQ(Movegen::perft(pos, 4), 199);
    EXPECT_EQ(Movegen::perft(pos, 5), 1347);
    EXPECT_EQ(Movegen::perft(pos, 6), 6249);

    pos.setFromFEN("8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 35);
    EXPECT_EQ(Movegen::perft(pos, 3), 182);
    EXPECT_EQ(Movegen::perft(pos, 4), 1091);
    EXPECT_EQ(Movegen::perft(pos, 5), 5408);
    EXPECT_EQ(Movegen::perft(pos, 6), 34822);

    pos.setFromFEN("8/8/8/8/8/4k3/4P3/4K3 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 2);
    EXPECT_EQ(Movegen::perft(pos, 2), 8);
    EXPECT_EQ(Movegen::perft(pos, 3), 44);
    EXPECT_EQ(Movegen::perft(pos, 4), 282);
    EXPECT_EQ(Movegen::perft(pos, 5), 1814);
    EXPECT_EQ(Movegen::perft(pos, 6), 11848);

    pos.setFromFEN("4k3/4p3/4K3/8/8/8/8/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 2);
    EXPECT_EQ(Movegen::perft(pos, 2), 8);
    EXPECT_EQ(Movegen::perft(pos, 3), 44);
    EXPECT_EQ(Movegen::perft(pos, 4), 282);
    EXPECT_EQ(Movegen::perft(pos, 5), 1814);
    EXPECT_EQ(Movegen::perft(pos, 6), 11848);

    pos.setFromFEN("8/8/7k/7p/7P/7K/8/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 9);
    EXPECT_EQ(Movegen::perft(pos, 3), 57);
    EXPECT_EQ(Movegen::perft(pos, 4), 360);
    EXPECT_EQ(Movegen::perft(pos, 5), 1969);
    EXPECT_EQ(Movegen::perft(pos, 6), 10724);

    pos.setFromFEN("8/8/k7/p7/P7/K7/8/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 9);
    EXPECT_EQ(Movegen::perft(pos, 3), 57);
    EXPECT_EQ(Movegen::perft(pos, 4), 360);
    EXPECT_EQ(Movegen::perft(pos, 5), 1969);
    EXPECT_EQ(Movegen::perft(pos, 6), 10724);

    pos.setFromFEN("8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 25);
    EXPECT_EQ(Movegen::perft(pos, 3), 180);
    EXPECT_EQ(Movegen::perft(pos, 4), 1294);
    EXPECT_EQ(Movegen::perft(pos, 5), 8296);
    EXPECT_EQ(Movegen::perft(pos, 6), 53138);

    pos.setFromFEN("8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 8);
    EXPECT_EQ(Movegen::perft(pos, 2), 61);
    EXPECT_EQ(Movegen::perft(pos, 3), 483);
    EXPECT_EQ(Movegen::perft(pos, 4), 3213);
    EXPECT_EQ(Movegen::perft(pos, 5), 23599);
    EXPECT_EQ(Movegen::perft(pos, 6), 157093);

    pos.setFromFEN("8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 8);
    EXPECT_EQ(Movegen::perft(pos, 2), 61);
    EXPECT_EQ(Movegen::perft(pos, 3), 411);
    EXPECT_EQ(Movegen::perft(pos, 4), 3213);
    EXPECT_EQ(Movegen::perft(pos, 5), 21637);
    EXPECT_EQ(Movegen::perft(pos, 6), 158065);

    pos.setFromFEN("k7/8/3p4/8/3P4/8/8/7K w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 15);
    EXPECT_EQ(Movegen::perft(pos, 3), 90);
    EXPECT_EQ(Movegen::perft(pos, 4), 534);
    EXPECT_EQ(Movegen::perft(pos, 5), 3450);
    EXPECT_EQ(Movegen::perft(pos, 6), 20960);

    pos.setFromFEN("8/8/7k/7p/7P/7K/8/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 9);
    EXPECT_EQ(Movegen::perft(pos, 3), 57);
    EXPECT_EQ(Movegen::perft(pos, 4), 360);
    EXPECT_EQ(Movegen::perft(pos, 5), 1969);
    EXPECT_EQ(Movegen::perft(pos, 6), 10724);

    pos.setFromFEN("8/8/k7/p7/P7/K7/8/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 9);
    EXPECT_EQ(Movegen::perft(pos, 3), 57);
    EXPECT_EQ(Movegen::perft(pos, 4), 360);
    EXPECT_EQ(Movegen::perft(pos, 5), 1969);
    EXPECT_EQ(Movegen::perft(pos, 6), 10724);

    pos.setFromFEN("8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 25);
    EXPECT_EQ(Movegen::perft(pos, 3), 180);
    EXPECT_EQ(Movegen::perft(pos, 4), 1294);
    EXPECT_EQ(Movegen::perft(pos, 5), 8296);
    EXPECT_EQ(Movegen::perft(pos, 6), 53138);

    pos.setFromFEN("8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 8);
    EXPECT_EQ(Movegen::perft(pos, 2), 61);
    EXPECT_EQ(Movegen::perft(pos, 3), 411);
    EXPECT_EQ(Movegen::perft(pos, 4), 3213);
    EXPECT_EQ(Movegen::perft(pos, 5), 21637);
    EXPECT_EQ(Movegen::perft(pos, 6), 158065);

    pos.setFromFEN("8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 8);
    EXPECT_EQ(Movegen::perft(pos, 2), 61);
    EXPECT_EQ(Movegen::perft(pos, 3), 483);
    EXPECT_EQ(Movegen::perft(pos, 4), 3213);
    EXPECT_EQ(Movegen::perft(pos, 5), 23599);
    EXPECT_EQ(Movegen::perft(pos, 6), 157093);

    pos.setFromFEN("k7/8/3p4/8/3P4/8/8/7K b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 15);
    EXPECT_EQ(Movegen::perft(pos, 3), 89);
    EXPECT_EQ(Movegen::perft(pos, 4), 537);
    EXPECT_EQ(Movegen::perft(pos, 5), 3309);
    EXPECT_EQ(Movegen::perft(pos, 6), 21104);

    pos.setFromFEN("7k/3p4/8/8/3P4/8/8/K7 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 19);
    EXPECT_EQ(Movegen::perft(pos, 3), 117);
    EXPECT_EQ(Movegen::perft(pos, 4), 720);
    EXPECT_EQ(Movegen::perft(pos, 5), 4661);
    EXPECT_EQ(Movegen::perft(pos, 6), 32191);

    pos.setFromFEN("7k/8/8/3p4/8/8/3P4/K7 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 19);
    EXPECT_EQ(Movegen::perft(pos, 3), 116);
    EXPECT_EQ(Movegen::perft(pos, 4), 716);
    EXPECT_EQ(Movegen::perft(pos, 5), 4786);
    EXPECT_EQ(Movegen::perft(pos, 6), 30980);

    pos.setFromFEN("k7/8/8/7p/6P1/8/8/K7 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 22);
    EXPECT_EQ(Movegen::perft(pos, 3), 139);
    EXPECT_EQ(Movegen::perft(pos, 4), 877);
    EXPECT_EQ(Movegen::perft(pos, 5), 6112);
    EXPECT_EQ(Movegen::perft(pos, 6), 41874);

    pos.setFromFEN("k7/8/7p/8/8/6P1/8/K7 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 16);
    EXPECT_EQ(Movegen::perft(pos, 3), 101);
    EXPECT_EQ(Movegen::perft(pos, 4), 637);
    EXPECT_EQ(Movegen::perft(pos, 5), 4354);
    EXPECT_EQ(Movegen::perft(pos, 6), 29679);

    pos.setFromFEN("k7/8/8/6p1/7P/8/8/K7 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 22);
    EXPECT_EQ(Movegen::perft(pos, 3), 139);
    EXPECT_EQ(Movegen::perft(pos, 4), 877);
    EXPECT_EQ(Movegen::perft(pos, 5), 6112);
    EXPECT_EQ(Movegen::perft(pos, 6), 41874);

    pos.setFromFEN("k7/8/6p1/8/8/7P/8/K7 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 16);
    EXPECT_EQ(Movegen::perft(pos, 3), 101);
    EXPECT_EQ(Movegen::perft(pos, 4), 637);
    EXPECT_EQ(Movegen::perft(pos, 5), 4354);
    EXPECT_EQ(Movegen::perft(pos, 6), 29679);

    pos.setFromFEN("k7/8/8/3p4/4p3/8/8/7K w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 3);
    EXPECT_EQ(Movegen::perft(pos, 2), 15);
    EXPECT_EQ(Movegen::perft(pos, 3), 84);
    EXPECT_EQ(Movegen::perft(pos, 4), 573);
    EXPECT_EQ(Movegen::perft(pos, 5), 3013);
    EXPECT_EQ(Movegen::perft(pos, 6), 22886);

    pos.setFromFEN("k7/8/3p4/8/8/4P3/8/7K w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 16);
    EXPECT_EQ(Movegen::perft(pos, 3), 101);
    EXPECT_EQ(Movegen::perft(pos, 4), 637);
    EXPECT_EQ(Movegen::perft(pos, 5), 4271);
    EXPECT_EQ(Movegen::perft(pos, 6), 28662);

    pos.setFromFEN("7k/3p4/8/8/3P4/8/8/K7 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 19);
    EXPECT_EQ(Movegen::perft(pos, 3), 117);
    EXPECT_EQ(Movegen::perft(pos, 4), 720);
    EXPECT_EQ(Movegen::perft(pos, 5), 5014);
    EXPECT_EQ(Movegen::perft(pos, 6), 32167);

    pos.setFromFEN("7k/8/8/3p4/8/8/3P4/K7 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 19);
    EXPECT_EQ(Movegen::perft(pos, 3), 117);
    EXPECT_EQ(Movegen::perft(pos, 4), 712);
    EXPECT_EQ(Movegen::perft(pos, 5), 4658);
    EXPECT_EQ(Movegen::perft(pos, 6), 30749);

    pos.setFromFEN("k7/8/8/7p/6P1/8/8/K7 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 22);
    EXPECT_EQ(Movegen::perft(pos, 3), 139);
    EXPECT_EQ(Movegen::perft(pos, 4), 877);
    EXPECT_EQ(Movegen::perft(pos, 5), 6112);
    EXPECT_EQ(Movegen::perft(pos, 6), 41874);

    pos.setFromFEN("k7/8/7p/8/8/6P1/8/K7 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 16);
    EXPECT_EQ(Movegen::perft(pos, 3), 101);
    EXPECT_EQ(Movegen::perft(pos, 4), 637);
    EXPECT_EQ(Movegen::perft(pos, 5), 4354);
    EXPECT_EQ(Movegen::perft(pos, 6), 29679);

    pos.setFromFEN("k7/8/8/6p1/7P/8/8/K7 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 22);
    EXPECT_EQ(Movegen::perft(pos, 3), 139);
    EXPECT_EQ(Movegen::perft(pos, 4), 877);
    EXPECT_EQ(Movegen::perft(pos, 5), 6112);
    EXPECT_EQ(Movegen::perft(pos, 6), 41874);

    pos.setFromFEN("k7/8/6p1/8/8/7P/8/K7 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 16);
    EXPECT_EQ(Movegen::perft(pos, 3), 101);
    EXPECT_EQ(Movegen::perft(pos, 4), 637);
    EXPECT_EQ(Movegen::perft(pos, 5), 4354);
    EXPECT_EQ(Movegen::perft(pos, 6), 29679);

    pos.setFromFEN("k7/8/8/3p4/4p3/8/8/7K b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 15);
    EXPECT_EQ(Movegen::perft(pos, 3), 102);
    EXPECT_EQ(Movegen::perft(pos, 4), 569);
    EXPECT_EQ(Movegen::perft(pos, 5), 4337);
    EXPECT_EQ(Movegen::perft(pos, 6), 22579);

    pos.setFromFEN("k7/8/3p4/8/8/4P3/8/7K b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 16);
    EXPECT_EQ(Movegen::perft(pos, 3), 101);
    EXPECT_EQ(Movegen::perft(pos, 4), 637);
    EXPECT_EQ(Movegen::perft(pos, 5), 4271);
    EXPECT_EQ(Movegen::perft(pos, 6), 28662);

    pos.setFromFEN("7k/8/8/p7/1P6/8/8/7K w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 22);
    EXPECT_EQ(Movegen::perft(pos, 3), 139);
    EXPECT_EQ(Movegen::perft(pos, 4), 877);
    EXPECT_EQ(Movegen::perft(pos, 5), 6112);
    EXPECT_EQ(Movegen::perft(pos, 6), 41874);

    pos.setFromFEN("7k/8/p7/8/8/1P6/8/7K w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 16);
    EXPECT_EQ(Movegen::perft(pos, 3), 101);
    EXPECT_EQ(Movegen::perft(pos, 4), 637);
    EXPECT_EQ(Movegen::perft(pos, 5), 4354);
    EXPECT_EQ(Movegen::perft(pos, 6), 29679);

    pos.setFromFEN("7k/8/8/1p6/P7/8/8/7K w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 22);
    EXPECT_EQ(Movegen::perft(pos, 3), 139);
    EXPECT_EQ(Movegen::perft(pos, 4), 877);
    EXPECT_EQ(Movegen::perft(pos, 5), 6112);
    EXPECT_EQ(Movegen::perft(pos, 6), 41874);

    pos.setFromFEN("7k/8/1p6/8/8/P7/8/7K w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 16);
    EXPECT_EQ(Movegen::perft(pos, 3), 101);
    EXPECT_EQ(Movegen::perft(pos, 4), 637);
    EXPECT_EQ(Movegen::perft(pos, 5), 4354);
    EXPECT_EQ(Movegen::perft(pos, 6), 29679);

    pos.setFromFEN("k7/7p/8/8/8/8/6P1/K7 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 25);
    EXPECT_EQ(Movegen::perft(pos, 3), 161);
    EXPECT_EQ(Movegen::perft(pos, 4), 1035);
    EXPECT_EQ(Movegen::perft(pos, 5), 7574);
    EXPECT_EQ(Movegen::perft(pos, 6), 55338);

    pos.setFromFEN("k7/6p1/8/8/8/8/7P/K7 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 25);
    EXPECT_EQ(Movegen::perft(pos, 3), 161);
    EXPECT_EQ(Movegen::perft(pos, 4), 1035);
    EXPECT_EQ(Movegen::perft(pos, 5), 7574);
    EXPECT_EQ(Movegen::perft(pos, 6), 55338);

    pos.setFromFEN("3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 7);
    EXPECT_EQ(Movegen::perft(pos, 2), 49);
    EXPECT_EQ(Movegen::perft(pos, 3), 378);
    EXPECT_EQ(Movegen::perft(pos, 4), 2902);
    EXPECT_EQ(Movegen::perft(pos, 5), 24122);
    EXPECT_EQ(Movegen::perft(pos, 6), 199002);

    pos.setFromFEN("7k/8/8/p7/1P6/8/8/7K b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 22);
    EXPECT_EQ(Movegen::perft(pos, 3), 139);
    EXPECT_EQ(Movegen::perft(pos, 4), 877);
    EXPECT_EQ(Movegen::perft(pos, 5), 6112);
    EXPECT_EQ(Movegen::perft(pos, 6), 41874);

    pos.setFromFEN("7k/8/p7/8/8/1P6/8/7K b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 16);
    EXPECT_EQ(Movegen::perft(pos, 3), 101);
    EXPECT_EQ(Movegen::perft(pos, 4), 637);
    EXPECT_EQ(Movegen::perft(pos, 5), 4354);
    EXPECT_EQ(Movegen::perft(pos, 6), 29679);

    pos.setFromFEN("7k/8/8/1p6/P7/8/8/7K b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 22);
    EXPECT_EQ(Movegen::perft(pos, 3), 139);
    EXPECT_EQ(Movegen::perft(pos, 4), 877);
    EXPECT_EQ(Movegen::perft(pos, 5), 6112);
    EXPECT_EQ(Movegen::perft(pos, 6), 41874);

    pos.setFromFEN("7k/8/1p6/8/8/P7/8/7K b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 4);
    EXPECT_EQ(Movegen::perft(pos, 2), 16);
    EXPECT_EQ(Movegen::perft(pos, 3), 101);
    EXPECT_EQ(Movegen::perft(pos, 4), 637);
    EXPECT_EQ(Movegen::perft(pos, 5), 4354);
    EXPECT_EQ(Movegen::perft(pos, 6), 29679);

    pos.setFromFEN("k7/7p/8/8/8/8/6P1/K7 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 25);
    EXPECT_EQ(Movegen::perft(pos, 3), 161);
    EXPECT_EQ(Movegen::perft(pos, 4), 1035);
    EXPECT_EQ(Movegen::perft(pos, 5), 7574);
    EXPECT_EQ(Movegen::perft(pos, 6), 55338);

    pos.setFromFEN("k7/6p1/8/8/8/8/7P/K7 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 5);
    EXPECT_EQ(Movegen::perft(pos, 2), 25);
    EXPECT_EQ(Movegen::perft(pos, 3), 161);
    EXPECT_EQ(Movegen::perft(pos, 4), 1035);
    EXPECT_EQ(Movegen::perft(pos, 5), 7574);
    EXPECT_EQ(Movegen::perft(pos, 6), 55338);

    pos.setFromFEN("3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 7);
    EXPECT_EQ(Movegen::perft(pos, 2), 49);
    EXPECT_EQ(Movegen::perft(pos, 3), 378);
    EXPECT_EQ(Movegen::perft(pos, 4), 2902);
    EXPECT_EQ(Movegen::perft(pos, 5), 24122);
    EXPECT_EQ(Movegen::perft(pos, 6), 199002);

    pos.setFromFEN("8/Pk6/8/8/8/8/6Kp/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 11);
    EXPECT_EQ(Movegen::perft(pos, 2), 97);
    EXPECT_EQ(Movegen::perft(pos, 3), 887);
    EXPECT_EQ(Movegen::perft(pos, 4), 8048);
    EXPECT_EQ(Movegen::perft(pos, 5), 90606);
    EXPECT_EQ(Movegen::perft(pos, 6), 1030499);

    pos.setFromFEN("n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 24);
    EXPECT_EQ(Movegen::perft(pos, 2), 421);
    EXPECT_EQ(Movegen::perft(pos, 3), 7421);
    EXPECT_EQ(Movegen::perft(pos, 4), 124608);
    EXPECT_EQ(Movegen::perft(pos, 5), 2193768);
    EXPECT_EQ(Movegen::perft(pos, 6), 37665329);

    pos.setFromFEN("8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 18);
    EXPECT_EQ(Movegen::perft(pos, 2), 270);
    EXPECT_EQ(Movegen::perft(pos, 3), 4699);
    EXPECT_EQ(Movegen::perft(pos, 4), 79355);
    EXPECT_EQ(Movegen::perft(pos, 5), 1533145);
    EXPECT_EQ(Movegen::perft(pos, 6), 28859283);

    pos.setFromFEN("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 24);
    EXPECT_EQ(Movegen::perft(pos, 2), 496);
    EXPECT_EQ(Movegen::perft(pos, 3), 9483);
    EXPECT_EQ(Movegen::perft(pos, 4), 182838);
    EXPECT_EQ(Movegen::perft(pos, 5), 3605103);
    EXPECT_EQ(Movegen::perft(pos, 6), 71179139);

    pos.setFromFEN("8/Pk6/8/8/8/8/6Kp/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 11);
    EXPECT_EQ(Movegen::perft(pos, 2), 97);
    EXPECT_EQ(Movegen::perft(pos, 3), 887);
    EXPECT_EQ(Movegen::perft(pos, 4), 8048);
    EXPECT_EQ(Movegen::perft(pos, 5), 90606);
    EXPECT_EQ(Movegen::perft(pos, 6), 1030499);

    pos.setFromFEN("n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 24);
    EXPECT_EQ(Movegen::perft(pos, 2), 421);
    EXPECT_EQ(Movegen::perft(pos, 3), 7421);
    EXPECT_EQ(Movegen::perft(pos, 4), 124608);
    EXPECT_EQ(Movegen::perft(pos, 5), 2193768);
    EXPECT_EQ(Movegen::perft(pos, 6), 37665329);

    pos.setFromFEN("8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 18);
    EXPECT_EQ(Movegen::perft(pos, 2), 270);
    EXPECT_EQ(Movegen::perft(pos, 3), 4699);
    EXPECT_EQ(Movegen::perft(pos, 4), 79355);
    EXPECT_EQ(Movegen::perft(pos, 5), 1533145);
    EXPECT_EQ(Movegen::perft(pos, 6), 28859283);

    pos.setFromFEN("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
    EXPECT_EQ(Movegen::perft(pos, 1), 24);
    EXPECT_EQ(Movegen::perft(pos, 2), 496);
    EXPECT_EQ(Movegen::perft(pos, 3), 9483);
    EXPECT_EQ(Movegen::perft(pos, 4), 182838);
    EXPECT_EQ(Movegen::perft(pos, 5), 3605103);
    EXPECT_EQ(Movegen::perft(pos, 6), 71179139);
}
