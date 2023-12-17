#include "Day5.h"
#include "File.h"
#include "Utils.h"
#include <cassert>

namespace
{
    class Map
    {
    public:
        void AddEntry(uint64_t destination, uint64_t source, uint64_t range)
        {
            Entries.push_back({ destination, source, range });
        }

        uint64_t SourceToDestination(uint64_t source)
        {
            for (const MapEntry& entry : Entries)
            {
                if (source >= entry.Source && source < (entry.Source + entry.Range))
                {
                    return entry.Destination + (source - entry.Source);
                }
            }

            return source;
        }

        uint64_t DestinationToSource(uint64_t destination)
        {
            for (const MapEntry& entry : Entries)
            {
                if (destination >= entry.Destination && destination < (entry.Destination + entry.Range))
                {
                    return entry.Source + (destination - entry.Destination);
                }
            }

            return destination;
        }

    private:
        struct MapEntry
        {
            uint64_t Destination;
            uint64_t Source;
            uint64_t Range;
        };

        std::vector<MapEntry> Entries;
    };

    std::vector<uint64_t> Seeds;

    Map SeedToSoilMap;
    Map SoilToFertilizerMap;
    Map FertilizerToWaterMap;
    Map WaterToLightMap;
    Map LightToTemperatureMap;
    Map TemperatureToHumidityMap;
    Map HumidityToLocationMap;

    void ParseMap(File& file, Map& outMap)
    {
        while(!file.IsLineEmpty() && !file.IsAtEnd())
        {
            const int kLineBufSize = 256;
            char lineBuf[kLineBufSize];

            file.ReadLine(lineBuf, kLineBufSize);

            const int kNumBufSize = 16;
            const int kMaxNums = 3;
            char nums[kMaxNums][kNumBufSize];

            int numNums = Utils::Tokenise(lineBuf, " ", &nums[0][0], kMaxNums, kNumBufSize);
            assert(numNums == 3);
            
            char* pEnd;
            outMap.AddEntry(strtoull(nums[0], &pEnd, 10), strtoull(nums[1], &pEnd, 10), strtoull(nums[2], &pEnd, 10));

            file.NextLine();
        }

        file.NextLine();
    }

    void ParseMaps(File& file)
    {
        assert(file.LineStartsWith("seed-to-soil map:"));
        file.NextLine();
        ParseMap(file, SeedToSoilMap);

        assert(file.LineStartsWith("soil-to-fertilizer map:"));
        file.NextLine();
        ParseMap(file, SoilToFertilizerMap);

        assert(file.LineStartsWith("fertilizer-to-water map:"));
        file.NextLine();
        ParseMap(file, FertilizerToWaterMap);

        assert(file.LineStartsWith("water-to-light map:"));
        file.NextLine();
        ParseMap(file, WaterToLightMap);

        assert(file.LineStartsWith("light-to-temperature map:"));
        file.NextLine();
        ParseMap(file, LightToTemperatureMap);

        assert(file.LineStartsWith("temperature-to-humidity map:"));
        file.NextLine();
        ParseMap(file, TemperatureToHumidityMap);

        assert(file.LineStartsWith("humidity-to-location map:"));
        file.NextLine();
        ParseMap(file, HumidityToLocationMap);

        assert(file.IsAtEnd());
    }

    void ParseSeeds(File& file)
    {
        assert(file.LineStartsWith("seeds: "));
        file.NextChar(strlen("seeds: "));

        const int kLineBufSize = 256;
        char lineBuf[kLineBufSize];

        file.ReadLine(lineBuf, kLineBufSize);

        const int kSeedBufSize = 16;
        const int kMaxSeeds = 32;
        char seeds[kMaxSeeds][kSeedBufSize];

        int numSeeds = Utils::Tokenise(lineBuf, " ", &seeds[0][0], kMaxSeeds, kSeedBufSize);

        for (int i = 0; i < numSeeds; ++i)
        {
            Seeds.push_back(atoi(seeds[i]));
        }

        file.NextLine(2);
    }

    uint64_t GetSeedLocation(uint64_t seed)
    {
        uint64_t soil = SeedToSoilMap.SourceToDestination(seed);
        uint64_t fertilizer = SoilToFertilizerMap.SourceToDestination(soil);
        uint64_t water = FertilizerToWaterMap.SourceToDestination(fertilizer);
        uint64_t light = WaterToLightMap.SourceToDestination(water);
        uint64_t temperature = LightToTemperatureMap.SourceToDestination(light);
        uint64_t humidity = TemperatureToHumidityMap.SourceToDestination(temperature);
        uint64_t location = HumidityToLocationMap.SourceToDestination(humidity);

        return location;
    }

    uint64_t GetLocationSeed(uint64_t location)
    {
        uint64_t humidity = HumidityToLocationMap.DestinationToSource(location);
        uint64_t temperature = TemperatureToHumidityMap.DestinationToSource(humidity);
        uint64_t light = LightToTemperatureMap.DestinationToSource(temperature);
        uint64_t water = WaterToLightMap.DestinationToSource(light);
        uint64_t fertilizer = FertilizerToWaterMap.DestinationToSource(water);
        uint64_t soil = SoilToFertilizerMap.DestinationToSource(fertilizer);
        uint64_t seed = SeedToSoilMap.DestinationToSource(soil);

        return seed;
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        File file(pDataFileName);

        ParseSeeds(file);
        ParseMaps(file);

        uint64_t lowest = -1;

        for (int i = 0; i < Seeds.size(); ++i)
        {
            uint64_t location = GetSeedLocation(Seeds[i]);
            lowest = std::min(lowest, location);
        }

        printf("Lowest Location: %llu\n", lowest);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        File file(pDataFileName);

        ParseSeeds(file);
        ParseMaps(file);

        uint64_t lowest = -1;

        //Faster to work backwards until we find the first location that matches a seed.
        for (uint64_t location = 0; lowest == -1; ++location)
        {
            uint64_t seedFound = GetLocationSeed(location);

            for (int i = 0; i < Seeds.size(); i = (i + 2))
            {
                if (seedFound >= Seeds[i] && seedFound < (Seeds[i] + Seeds[i + 1]))
                {
                    lowest = location;
                    break;
                }
            }
        }

        printf("Lowest Location: %llu\n", lowest);
    }
}


void Day5::Run()
{
    //RunPuzzle1("Day5_Sample.txt");
    //RunPuzzle1("Day5_Puzzle.txt");

    //RunPuzzle2("Day5_Sample.txt");
    RunPuzzle2("Day5_Puzzle.txt");
}
