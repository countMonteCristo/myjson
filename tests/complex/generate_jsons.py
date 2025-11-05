import enum
import json
import random
import sys
from typing import Any, Dict, List, Optional, Union
from string import printable


class JsonType(enum.Enum):
    NONE = 0
    BOOL = 1
    NUMBER = 2
    STRING = 3
    ARRAY = 4
    OBJECT = 5

JSON_TYPES = tuple(JsonType)
MAX_DEPTH = 2

def get_random_number() -> int|float:
    if random.random() < 0.5:
        return random.random() * 1_000_000_000
    else:
        return random.randint(-1_000_000_000, 1_000_000_000)

def get_random_string() -> str:
    return ''.join(random.choices(printable, k = random.randint(1, 20)))

def get_random_array(depth: int) -> List[Any]:
    result: List[Any] = []
    if depth >= MAX_DEPTH:
        return result
    for _ in range(random.randint(0, 20)):
        current_type = random.choice(JSON_TYPES)
        value: Optional[Union[int, float, str, List[Any], Dict[str, Any]]] = None
        match current_type:
            case JsonType.NONE: value = None
            case JsonType.BOOL: value = random.choice((True, False))
            case JsonType.NUMBER: value = get_random_number()
            case JsonType.STRING: value = get_random_string()
            case JsonType.ARRAY: value = get_random_array(depth=depth + 1)
            case JsonType.OBJECT: value = get_random_object(depth=depth + 1)
        result.append(value)
    return result

def get_random_object(depth: int) -> Dict[str, Any]:
    result: Dict[str, Any] = {}
    if depth >= MAX_DEPTH:
        return result
    for _ in range(random.randint(0, 20)):
        current_type = random.choice(JSON_TYPES)
        key = get_random_string()
        value: Optional[Union[int, float, str, List[Any], Dict[str, Any]]] = None
        match current_type:
            case JsonType.NONE: value = None
            case JsonType.BOOL: value = random.choice((True, False))
            case JsonType.NUMBER: value = get_random_number()
            case JsonType.STRING: value = get_random_string()
            case JsonType.ARRAY: value = get_random_array(depth=depth + 1)
            case JsonType.OBJECT: value = get_random_object(depth=depth + 1)
        result[key] = value
    return result


def generate_random_json() -> None:
    current_type = random.choice(JSON_TYPES)
    result: Optional[Union[int, float, str, List[Any], Dict[str, Any]]] = None
    match current_type:
        case JsonType.NONE: result = None
        case JsonType.BOOL: result = random.choice((True, False))
        case JsonType.NUMBER: result = get_random_number()
        case JsonType.STRING: result = get_random_string()
        case JsonType.ARRAY: result = get_random_array(depth=0)
        case JsonType.OBJECT: result = get_random_object(depth=0)
    print(json.dumps(result))

def main() -> int:
    N = int(sys.argv[1])
    seed = 0 if len(sys.argv) < 3 else int(sys.argv[2])
    random.seed(seed)
    for _ in range(N):
        generate_random_json()
    return 0

if __name__ == "__main__":
    sys.exit(main())
