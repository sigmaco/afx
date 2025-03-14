// Generated automatically from src/optional/xssOpt_random.xss. Do not edit.
static const char* randomModuleSource =
"foreign class Random {\n"
"  construct new() {\n"
"    seed_()\n"
"  }\n"
"\n"
"  construct new(seed) {\n"
"    if (seed is Num) {\n"
"      seed_(seed)\n"
"    } else if (seed is Sequence) {\n"
"      if (seed.isEmpty) Fiber.abort(\"Sequence cannot be empty.\")\n"
"\n"
"      // TODO: Empty sequence.\n"
"      var seeds = []\n"
"      for (element in seed) {\n"
"        if (!(element is Num)) Fiber.abort(\"Sequence elements must all be numbers.\")\n"
"\n"
"        seeds.add(element)\n"
"        if (seeds.count == 16) break\n"
"      }\n"
"\n"
"      // Cycle the values to fill in any missing slots.\n"
"      var i = 0\n"
"      while (seeds.count < 16) {\n"
"        seeds.add(seeds[i])\n"
"        i = i + 1\n"
"      }\n"
"\n"
"      seed_(\n"
"          seeds[0], seeds[1], seeds[2], seeds[3],\n"
"          seeds[4], seeds[5], seeds[6], seeds[7],\n"
"          seeds[8], seeds[9], seeds[10], seeds[11],\n"
"          seeds[12], seeds[13], seeds[14], seeds[15])\n"
"    } else {\n"
"      Fiber.abort(\"Seed must be a number or a sequence of numbers.\")\n"
"    }\n"
"  }\n"
"\n"
"  foreign seed_()\n"
"  foreign seed_(seed)\n"
"  foreign seed_(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16)\n"
"\n"
"  foreign float()\n"
"  float(end) { float() * end }\n"
"  float(start, end) { float() * (end - start) + start }\n"
"\n"
"  foreign int()\n"
"  int(end) { (float() * end).floor }\n"
"  int(start, end) { (float() * (end - start)).floor + start }\n"
"\n"
"  sample(list) {\n"
"    if (list.count == 0) Fiber.abort(\"Not enough elements to sample.\")\n"
"    return list[int(list.count)]\n"
"  }\n"
"  sample(list, count) {\n"
"    if (count > list.count) Fiber.abort(\"Not enough elements to sample.\")\n"
"\n"
"    var result = []\n"
"\n"
"    // The algorithm described in \"Programming pearls: a sample of brilliance\".\n"
"    // Use a hash map for sample sizes less than 1/4 of the population size and\n"
"    // an array of booleans for larger samples. This simple heuristic improves\n"
"    // performance for large sample sizes as well as reduces memory usage.\n"
"    if (count * 4 < list.count) {\n"
"      var picked = {}\n"
"      for (i in list.count - count...list.count) {\n"
"        var index = int(i + 1)\n"
"        if (picked.containsKey(index)) index = i\n"
"        picked[index] = true\n"
"        result.add(list[index])\n"
"      }\n"
"    } else {\n"
"      var picked = List.filled(list.count, false)\n"
"      for (i in list.count - count...list.count) {\n"
"        var index = int(i + 1)\n"
"        if (picked[index]) index = i\n"
"        picked[index] = true\n"
"        result.add(list[index])\n"
"      }\n"
"    }\n"
"\n"
"    return result\n"
"  }\n"
"\n"
"  shuffle(list) {\n"
"    if (list.isEmpty) return\n"
"\n"
"    // Fisher-Yates shuffle.\n"
"    for (i in 0...list.count - 1) {\n"
"      var from = int(i, list.count)\n"
"      var temp = list[from]\n"
"      list[from] = list[i]\n"
"      list[i] = temp\n"
"    }\n"
"  }\n"
"}\n";
