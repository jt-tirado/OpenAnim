#ifndef HUDDEFINES_H
#define HUDDEFINES_H

namespace DynamoMayaHud
{

#define McheckStatus(stat,msg)  \
    if (!stat) {                \
        stat.perror(msg);       \
        return stat;            \
    }
#define McheckVoid(stat,msg)    \
    if (!stat) {                \
        stat.perror(msg);       \
        return;                 \
    }

#define kStrUnknown     "n/a"
#define kStrUser        "{user}"
#define kStrScene       "{scene}"
#define kStrFile        "{file}"
#define kStrProject     "{project}"
#define kStrVersion     "{version}"
#define kStrCamera      "{camera}"
#define kStrFocal       "{focal}"

#define kStrFPS         "{fps}"
#define kStrTime        "{time}"
#define kStrTimecode    "{timecode}"
#define kStrFrame       "{frame}"

#define kStrDate        "{date}"
#define kStrDay         "{d}"
#define kStrMonth       "{m}"
#define kStrMonth2      "{M}"
#define kStrYear2       "{y}"
#define kStrYear4       "{Y}"

#define kStrTETrack0    "{teTrack0}"
#define kStrTETrack1    "{teTrack1}"
#define kStrTETrack2    "{teTrack2}"
#define kStrTETrack3    "{teTrack3}"

#define kStrTEClip      "{teClip}"

#define kStrDebug       "{user}, {file}, {scene}, {version}, {project}, {camera}, {focal}, {fps}, {time}, {timecode}, {frame}, {d}, {m}, {y}, {Y}, {date}"
}
#endif