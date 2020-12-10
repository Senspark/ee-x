package com.soomla;

import com.soomla.data.JSONConsts;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.List;

/**
 * Created by refaelos on 05/08/14.
 */
public class Schedule {
    private static String TAG = "SOOMLA Schedule";

    public enum Recurrence {
        EVERY_MONTH,
        EVERY_WEEK,
        EVERY_DAY,
        EVERY_HOUR,
        NONE
    }

    public class DateTimeRange {
        public Date Start;
        public Date End;
        public DateTimeRange(Date start, Date end) {
            Start = start;
            End = end;
        }
    }

    public static Schedule AnyTimeOnce() {
        return new Schedule(1);
    }

    public static Schedule AnyTimeLimited(int activationLimit) {
        return new Schedule(activationLimit);
    }

    public static Schedule AnyTimeUnLimited() {
        return new Schedule(0);
    }

    public Schedule(int activationLimit)
    {
        this(null, Recurrence.NONE, activationLimit);
    }

    public Schedule(final Date startTime, final Date endTime, Recurrence recurrence, int activationLimit)
    {
        List<DateTimeRange> ranges = new ArrayList<DateTimeRange>() {{ add(new DateTimeRange(startTime, endTime)); }};
        mTimeRanges = ranges;

        mRequiredRecurrence = recurrence;
        mActivationLimit = activationLimit;
    }

    public Schedule(List<DateTimeRange> timeRanges, Recurrence recurrence, int activationLimit)
    {
        mTimeRanges = timeRanges;

        mRequiredRecurrence = recurrence;
        mActivationLimit = activationLimit;
    }

    public Schedule(JSONObject jsonSched) throws JSONException
    {
        if(jsonSched.has(JSONConsts.SOOM_SCHE_REC)) {
            mRequiredRecurrence = Recurrence.values()[jsonSched.getInt(JSONConsts.SOOM_SCHE_REC)];
        } else {
            mRequiredRecurrence = Recurrence.NONE;
        }

        mActivationLimit = jsonSched.getInt(JSONConsts.SOOM_SCHE_APPROVALS);

        mTimeRanges = null;
        if (jsonSched.has(JSONConsts.SOOM_SCHE_RANGES)) {
            mTimeRanges = new ArrayList<DateTimeRange>();
            JSONArray rangesObjs = jsonSched.getJSONArray(JSONConsts.SOOM_SCHE_RANGES);
            for(int i=0; i<rangesObjs.length(); i++) {
                long tmpTimeMillis = rangesObjs.getJSONObject(i).getLong(JSONConsts.SOOM_SCHE_RANGE_START);
                Date start = new Date(tmpTimeMillis);
                tmpTimeMillis = rangesObjs.getJSONObject(i).getLong(JSONConsts.SOOM_SCHE_RANGE_END);
                Date end = new Date(tmpTimeMillis);

                mTimeRanges.add(new DateTimeRange(start, end));
            }
        }
    }

    public JSONObject toJSONObject() {
        JSONObject jsonObject = new JSONObject();

        try {
            if (mRequiredRecurrence != null) {
                jsonObject.put(JSONConsts.SOOM_SCHE_REC, mRequiredRecurrence.ordinal());
            }

            if (mTimeRanges != null) {
                JSONArray rangesObjs = new JSONArray();
                for(DateTimeRange range : mTimeRanges) {
                    long startMillis = range.Start.getTime();
                    long endMillis = range.End.getTime();
                    JSONObject rangeObj = new JSONObject();
                    rangeObj.put(JSONConsts.SOOM_CLASSNAME, SoomlaUtils.getClassName(range));
                    rangeObj.put(JSONConsts.SOOM_SCHE_RANGE_START, startMillis);
                    rangeObj.put(JSONConsts.SOOM_SCHE_RANGE_END, endMillis);
                    rangesObjs.put(rangeObj);
                }
                jsonObject.put(JSONConsts.SOOM_SCHE_RANGES, rangesObjs);
            }

            jsonObject.put(JSONConsts.SOOM_SCHE_APPROVALS, mActivationLimit);

            jsonObject.put(JSONConsts.SOOM_CLASSNAME, SoomlaUtils.getClassName(this));
        } catch (JSONException e) {
            SoomlaUtils.LogError(TAG, "An error occurred while generating JSON object.");
        }

        return jsonObject;
    }

    public boolean approve(int activationTimes) {

        Date now = new Date();

        if (mActivationLimit < 1 && (mTimeRanges == null || mTimeRanges.size() == 0)) {
            SoomlaUtils.LogDebug(TAG, "There's no activation limit and no TimeRanges. APPROVED!");
            return true;
        }

        if (mActivationLimit>0 && activationTimes >= mActivationLimit) {
            SoomlaUtils.LogDebug(TAG, "Activation limit exceeded.");
            return false;
        }

        if ((mTimeRanges == null || mTimeRanges.size() == 0)) {
            SoomlaUtils.LogDebug(TAG, "We have an activation limit that was not reached. Also, we don't have any time ranges. APPROVED!");
            return true;
        }


        // NOTE: From this point on ... we know that we didn't reach the activation limit AND we have TimeRanges.
        //		 We'll just make sure the time ranges and the Recurrence copmlies.


        for(DateTimeRange dtr : mTimeRanges) {
            if (now.after(dtr.Start) && now.before(dtr.End)) {
                SoomlaUtils.LogDebug(TAG, "We are just in one of the time spans, it can't get any better then that. APPROVED!");
                return true;
            }
        }

        // we don't need to continue if RequiredRecurrence is NONE
        if (mRequiredRecurrence == Recurrence.NONE) {
            return false;
        }

        for(DateTimeRange dtr : mTimeRanges) {
            Calendar nowCalendar = new GregorianCalendar();
            nowCalendar.setTime(now);
            Calendar startCalendar = new GregorianCalendar();
            startCalendar.setTime(dtr.Start);
            Calendar endCalendar = new GregorianCalendar();
            endCalendar.setTime(dtr.End);
            if (nowCalendar.get(Calendar.MINUTE) >= startCalendar.get(Calendar.MINUTE) &&
                    nowCalendar.get(Calendar.MINUTE) <= endCalendar.get(Calendar.MINUTE)) {
                SoomlaUtils.LogDebug(TAG, "Now is in one of the time ranges' minutes span.");

                if (mRequiredRecurrence == Recurrence.EVERY_HOUR) {
                    SoomlaUtils.LogDebug(TAG, "It's a EVERY_HOUR recurrence. APPROVED!");
                    return true;
                }

                if (nowCalendar.get(Calendar.HOUR) >= startCalendar.get(Calendar.HOUR) &&
                        nowCalendar.get(Calendar.HOUR) <= endCalendar.get(Calendar.HOUR)) {
                    SoomlaUtils.LogDebug(TAG, "Now is in one of the time ranges' hours span.");

                    if (mRequiredRecurrence == Recurrence.EVERY_DAY) {
                        SoomlaUtils.LogDebug(TAG, "It's a EVERY_DAY recurrence. APPROVED!");
                        return true;
                    }

                    if (nowCalendar.get(Calendar.DAY_OF_WEEK) >= startCalendar.get(Calendar.DAY_OF_WEEK)
                            && nowCalendar.get(Calendar.DAY_OF_WEEK) <= endCalendar.get(Calendar.DAY_OF_WEEK)) {
                        SoomlaUtils.LogDebug(TAG, "Now is in one of the time ranges' day-of-week span.");

                        if (mRequiredRecurrence == Recurrence.EVERY_WEEK) {
                            SoomlaUtils.LogDebug(TAG, "It's a EVERY_WEEK recurrence. APPROVED!");
                            return true;
                        }

                        if (nowCalendar.get(Calendar.DAY_OF_MONTH) >= startCalendar.get(Calendar.DAY_OF_MONTH)
                                && nowCalendar.get(Calendar.DAY_OF_MONTH) <= endCalendar.get(Calendar.DAY_OF_MONTH)) {
                            SoomlaUtils.LogDebug(TAG, "Now is in one of the time ranges' days span.");

                            if (mRequiredRecurrence == Recurrence.EVERY_MONTH) {
                                SoomlaUtils.LogDebug(TAG, "It's a EVERY_MONTH recurrence. APPROVED!");
                                return true;
                            }
                        }
                    }
                }
            }

        }

        return false;
    }

    public Recurrence getRequiredRecurrence() {
        return mRequiredRecurrence;
    }

    public List<DateTimeRange> getTimeRanges() {
        return mTimeRanges;
    }

    public int getActivationLimit() {
        return mActivationLimit;
    }

    public Recurrence mRequiredRecurrence;
    public List<DateTimeRange> mTimeRanges;
    public int mActivationLimit;

}
