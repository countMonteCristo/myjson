import json
import sys

def main() -> int:
    generated_fn = sys.argv[1]
    processed_fn = sys.argv[2]

    with open(generated_fn) as generated_istrem, open(processed_fn) as processed_istream:
        for i,(generated, processed) in enumerate(zip(generated_istrem, processed_istream), 1):
            gen_json = json.loads(generated)
            prc_json = json.loads(processed)
            if gen_json != prc_json:
                print(f"[TEST] Error at line {i}", file=sys.stderr)
                return 1
    return 0

if __name__ == "__main__":
    sys.exit(main())