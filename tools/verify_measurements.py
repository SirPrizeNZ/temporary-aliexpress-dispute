"""Check the published storage arithmetic and independent-result agreement."""
import json
from pathlib import Path

root = Path(__file__).resolve().parent.parent
m = json.loads((root / 'evidence/measurements.json').read_text())
assert m['flash']['sectors'] * m['flash']['sector_bytes'] == 15_762_194_432
native = m['native_statfs']
direct = m['direct_syscall_statfs']
assert native['return'] == direct['return'] == 0
assert native == direct
for volume in m['volumes']:
    for reported, raw in [('file_total', 'total'), ('file_usable', 'available'), ('file_free', 'free'), ('statfs_total', 'total'), ('statfs_available', 'available')]:
        assert volume[reported] - native[raw] == 48 * 1024**3, (volume['path'], reported)
assert native['available'] > m['low_threshold_bytes']
assert m['properties'] == {'persist.sys.rom': '64G', 'persist.sys.truerom': '16GB'}
print('PASS: flash = 15,762,194,432 bytes; native and kernel results agree.')
print('PASS: all comparable Android counts are inflated by exactly 48 GiB.')
print('PASS: recorded available space exceeds the recorded low-space threshold.')
