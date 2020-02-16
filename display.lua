format_version = "1.0"

local maxArraySize = 256
local midiNotes = {
  "C-2", "C#-2", "D-2", "D#-2", "E-2", "F-2", "F#-2", "G-2", "G#-2", "A-2", "A#-2", "B-2",
  "C-1", "C#-1", "D-1", "D#-1", "E-1", "F-1", "F#-1", "G-1", "G#-1", "A-1", "A#-1", "B-1",
  "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0",
  "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
  "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
  "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
  "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
  "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
  "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6",
  "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7",
  "C8", "C#8", "D8", "D#8", "E8", "F8", "F#8", "G8"
}

local colorTable = {
  [0] = { light = {r = 0,   g = 255, b = 255 }, dark = {r = 0,   g = 128, b = 128 }}, -- blue
  [1] = { light = {r = 0,   g = 255, b = 0   }, dark = {r = 0,   g = 128, b = 0   }}, -- green
  [2] = { light = {r = 255, g = 63,   b = 63 }, dark = {r = 160, g = 34,   b = 34   } }, -- red
  [3] = { light = {r = 255, g = 255, b = 0   }, dark = {r = 128, g = 128, b = 0   }}, -- yellow
  [4] = { light = {r = 255, g = 255, b = 255 }, dark = {r = 128, g = 128, b = 128 }}, -- white
}

local clippingColorTable = {
  [0] = {r = 255, g = 63,   b = 63 }, -- red
  [1] = {r = 255, g = 63,   b = 63 }, -- red
  [2] = {r = 255, g = 255, b = 255 }, -- white
  [3] = {r = 255, g = 63,   b = 63 }, -- red
  [4] = {r = 255, g = 63,   b = 63 }, -- red
}

-------------------------------------------------

local mainLCDWidth = 256
-- we use only the 1-100 for the drawing
local mainLCDHeight = 102

-------------------------------------------------
--- MainLCD
-------------------------------------------------
local propMainLCDCVInStateIdx = 1
local propMainLCDInputPausedIdx = 2
local propMainLCDInputPageOffsetIdx = 3
local propMainLCDCVInColorIdx = 4
local propMainLCDScreenOnIdx = 5
local propMainLCDPlus05AxisIdx = 6
local propMainLCDArrayStartIdx = 7
local propMainLCDArrayIdx = 8

local cvInStateNotConnected = 0
local cvInStateConnected = 1

local whiteColor = {r = 255, g = 255, b = 255 }
local whiteColor40 = {r = 255, g = 255, b = 255, a = 40 }
local whiteColor60 = {r = 255, g = 255, b = 255, a = 60 }
local redColor = {r = 255, g = 0, b = 0 }
local blackColor = {r = 0, g = 0, b = 0 }

local offColor = {r = 64, g = 64, b = 64 }

local textColor = whiteColor
local textFont = "Bold LCD font"

------------------
-- clamp
------------------
function clamp(v, min, max)
  if v < min then
    return min
  elseif v > max then
    return max
  else
    return v
  end
end

------------------
-- equalsTo
------------------
function equalsTo(v1, v2)
  return math.abs(v1 - v2) < 0.0000001
end

------------------
-- drawHorizontalLine
------------------
function drawHorizontalLine(y, color, startX, endX)
  if not startX then startX = 0 end
  if not endX then endX = mainLCDWidth end
  jbox_display.draw_line({x = startX, y = y}, {x = endX, y = y}, color)
end

--------------------
---- drawVerticalLine
--------------------
--function drawVerticalLine(x, color, startY, endY)
--  if not startY then startY = 0 end
--  if not endY then endY = mainLCDHeight end
--  jbox_display.draw_rect({left = x, top = startY, right = x + 1, bottom = endY}, color)
--end

------------------
-- drawPoints
------------------
function drawPoints(points, color)
  if #points > 0 then
    if #points >= 2 then
--      jbox_display.draw_lines(points, color, "open")
--[[
      local previousPoint
      for k, point in pairs(points) do
        if k > 1 then
          local vertices = {
            {x = previousPoint.x, y = previousPoint.yMin},
            {x = previousPoint.x, y = previousPoint.yMax},
            {x = point.x, y = point.yMin},
            {x = point.x, y = point.yMax},
          }
          jbox_display.draw_polygon(vertices, color)
        end
        previousPoint = point
      end
]]
      local vertices = {}
      for k, point in pairs(points) do
        vertices[#vertices + 1] = {x = point.x, y = point.yMax}
      end

      for k = #points, 1, -1 do
        local point = points[k]
        vertices[#vertices + 1] = {x = point.x, y = point.yMin}
      end

      jbox_display.draw_polygon(vertices, color)

    else
      jbox_display.draw_line(points[1], points[1], color)
    end
  end
end

------------------
-- drawCurveWhenClipping
------------------
function drawCurveWhenClipping(points, color, clippingColor)

  local ps = {}
  local previousPoint
  local clippingStartPoint

  for k, point in pairs(points) do

    -- first one (handle differently)
    if not previousPoint then
      if point.t == 0 then
        ps[#ps + 1] = point
      else
        clippingStartPoint = point
      end
    else
      -- same point type as previous one
      if previousPoint.t == point.t then
        if point.t == 0 then
          ps[#ps + 1] = point -- accumulate if it is a 'regular' point
        end
      else
        -- type changing
        if previousPoint.t == 0 then
          ps[#ps + 1] = point
          drawPoints(ps, color)
          ps = {}
          clippingStartPoint = point
        else
          if point.t == 0 then
            drawHorizontalLine(clippingStartPoint.y, clippingColor, clippingStartPoint.x, previousPoint.x)
            ps = { previousPoint, point }
            clippingStartPoint = nil
          else
            drawPoints({ previousPoint, point }, color)
            drawHorizontalLine(clippingStartPoint.y, clippingColor, clippingStartPoint.x, previousPoint.x)
            clippingStartPoint = point
          end
        end

      end
    end

    previousPoint = point

  end

  -- previousPoint represent the last point now and we need to handle the last section
  if clippingStartPoint then
    drawHorizontalLine(clippingStartPoint.y, clippingColor, clippingStartPoint.x, previousPoint.x)
  else
    drawPoints(ps, color)
  end

end

local zeroAxis = mainLCDHeight / 2
local zeroAxisColor = whiteColor60
local point5AxisColor = whiteColor40
local minusPoint5AxisColor = whiteColor40

local displayNegativeClipping = 110
local displayPositiveClipping = 120

------------------
-- MainLCDDraw
------------------
MainLCDDraw = function(props, di, dr)

  if props[propMainLCDCVInStateIdx] == cvInStateConnected and props[propMainLCDScreenOnIdx] then

    local cvIn1Color = colorTable[props[propMainLCDCVInColorIdx]]["light"]

    local inputPageOffset = props[propMainLCDInputPageOffsetIdx]
    local inputPageOffsetTop

    local idx = props[propMainLCDArrayStartIdx]
    local size = maxArraySize
    local left = 0

    -- axises
    local point5Axis = mainLCDHeight - props[propMainLCDPlus05AxisIdx]
    if point5Axis ~= zeroAxis then
      drawHorizontalLine(point5Axis + 0.5, point5AxisColor)
      -- symetric to zeroAxis
      local minusPoint5Axis = zeroAxis * 2 - point5Axis
      drawHorizontalLine(minusPoint5Axis + 0.5, minusPoint5AxisColor)
    end

    -- 0 axis
    drawHorizontalLine(zeroAxis + 0.5, zeroAxisColor)

    local cv

    local points = {}
    local clipping = false

    while size > 0 do

      -- we increase first as the latest entry should be at the right edge of the display => latest value to
      -- be displayed
      idx = idx + 1
      if idx >= maxArraySize then
        idx = 0
      end

      cv = props[propMainLCDArrayIdx + idx]

      local pointType = 0

      local max = math.floor(cv / 256)
      local min = cv - max * 256

--      if cv == displayNegativeClipping or cv == displayPositiveClipping then
--        pointType = cv
--        cv = cv - 200
--        clipping = true
--      end

      local topAvg = mainLCDHeight - 1 - cv -- we start at 1

      -- record the value for the input page offset (for pause mode)
      if left == inputPageOffset then
        inputPageOffsetTop = topAvg
      end

      points[#points + 1] = {x = left + 0.5, yAvg = topAvg + 0.5, yMin = mainLCDHeight - 0.5 - min, yMax = mainLCDHeight - 0.5 - max, t = pointType }

      left = left + 1
      size = size - 1
    end

    if clipping then
      drawCurveWhenClipping(points, cvIn1Color, clippingColorTable[props[propMainLCDCVInColorIdx]])
    else
      -- no clipping, simply draw the points
      drawPoints(points, cvIn1Color)
    end

    if props[propMainLCDInputPausedIdx] then
      jbox_display.draw_rect({left = inputPageOffset - 5, top = 0, right = inputPageOffset + 5, bottom = mainLCDHeight}, whiteColor40)
      jbox_display.draw_rect({left = inputPageOffset - 3, top = 0, right = inputPageOffset + 3, bottom = mainLCDHeight}, whiteColor60)
      jbox_display.draw_line({x = inputPageOffset, y = 0}, {x = inputPageOffset, y = mainLCDHeight}, whiteColor)
      jbox_display.draw_line({x = 0, y = inputPageOffsetTop}, {x = mainLCDWidth, y = inputPageOffsetTop}, whiteColor60)
      jbox_display.draw_rect({left = inputPageOffset - 3, top = inputPageOffsetTop - 3, right = inputPageOffset + 3, bottom = inputPageOffsetTop + 3}, whiteColor)
      jbox_display.draw_rect({left = inputPageOffset - 2, top = inputPageOffsetTop - 2, right = inputPageOffset + 2, bottom = inputPageOffsetTop + 2}, redColor)
    end
  else
    -- axises
    local point5Axis = mainLCDHeight - props[propMainLCDPlus05AxisIdx]
    if point5Axis ~= zeroAxis then
      drawHorizontalLine(point5Axis + 0.5, offColor)
      -- symetric to zeroAxis
      local minusPoint5Axis = zeroAxis * 2 - point5Axis
      drawHorizontalLine(minusPoint5Axis + 0.5, offColor)
    end

    -- 0 axis
    drawHorizontalLine(zeroAxis + 0.5, offColor)
  end
end

--------------------
---- MainLCDGesture
--------------------
MainLCDGestureHandler = function (props, di, gi, cd)
  local inputPageOffset = gi.current_point.x

  if inputPageOffset < 0 then inputPageOffset = 0 end
  if inputPageOffset >= mainLCDWidth then inputPageOffset = mainLCDWidth - 1 end

  local changes = {}
  if not props[propMainLCDInputPausedIdx] then
    changes[propMainLCDInputPausedIdx] = true
  end
  changes[propMainLCDInputPageOffsetIdx] = inputPageOffset
  return { gesture_ui_name = jbox.ui_text("undo prop_input_page_offset"), property_changes = changes }
end

MainLCDGesture = function(props, di, gsp)
  -- handle gesture only when connected and in pause mode...
  if props[propMainLCDCVInStateIdx] == cvInStateConnected and
      props[propMainLCDScreenOnIdx] then
    local gesture_definition = {
      custom_data = {},
      handlers = {
        on_tap = "MainLCDGestureHandler",
        on_update = "MainLCDGestureHandler"
      }
    }
    return gesture_definition
  else
    return nil
  end
end

------------------
-- MainLCDScrollbarDraw
------------------
local propMainLCDScrollbarCVInStateIdx = 1
local propMainLCDScrollbarInputPausedIdx = 2
local propMainLCDScrollbarInputHistoryOffsetIdx = 3
local propMainLCDScrollbarInputHistoryOffsetRTIdx = 4
local propMainLCDScrollbarZoomFactorXIdx = 5
local propMainLCDScrollbarCVInColordx = 6
local propMainLCDScrollbarScreenOnIdx = 7

local MIN_HISTORY_OFFSET = 0;
local MAX_HISTORY_OFFSET = 10000;
local IGNORE_HISTORY_OFFSET_RT = 10001;

function computeHistoryOffset(props)
  local inputHistoryOffset = props[propMainLCDScrollbarInputHistoryOffsetRTIdx]
  if inputHistoryOffset == IGNORE_HISTORY_OFFSET_RT then
    inputHistoryOffset = props[propMainLCDScrollbarInputHistoryOffsetIdx]
  end
  return inputHistoryOffset
end

function computeZoomBox(props)
  local zoomBoxSize = 256 - (239 * props[propMainLCDScrollbarZoomFactorXIdx])
  local minCenter = zoomBoxSize / 2
  local maxCenter = mainLCDWidth - (zoomBoxSize / 2)
  local inputHistoryOffset = computeHistoryOffset(props)
  local center = (maxCenter - minCenter) * inputHistoryOffset / MAX_HISTORY_OFFSET + minCenter
  return {
    minCenter = minCenter,
    maxCenter = maxCenter,
    center = center,
    left = center - (zoomBoxSize / 2),
    right = center + (zoomBoxSize / 2),
    size = zoomBoxSize
  }
end

function moveZoomBox(zoomBox, deltaX)
  local center = clamp(zoomBox.center + deltaX, zoomBox.minCenter, zoomBox.maxCenter)
  return {
    minCenter = zoomBox.minCenter,
    maxCenter = zoomBox.maxCenter,
    center = center,
    left = center - (zoomBox.size / 2),
    right = center + (zoomBox.size / 2),
    size = zoomBox.size
  }
end

function computeNewHistoryOffset(zoomBox)
  return (zoomBox.center - zoomBox.minCenter) * MAX_HISTORY_OFFSET / (zoomBox.maxCenter - zoomBox.minCenter)
end

------------------
-- MainLCDScrollbarDraw
------------------

local scrollBarBorderPoints = { { x = 0.5, y = 0.5 }, { x = 255.5, y = 0.5}, { x= 255.5, y = 15.5 }, {x = 0.5, y = 15.5 } }
local scrollBarBoder = 2.5
-- local scrollBarBorderPoints = { { x = 0, y = 0 }, { x = 256, y = 0}, { x = 256, y = 16 }, {x = 0, y = 16 }}

MainLCDScrollbarDraw = function(props, di, dr)

  if props[propMainLCDScrollbarCVInStateIdx] == cvInStateConnected and props[propMainLCDScrollbarScreenOnIdx] then

    local box = computeZoomBox(props)
    local cvIn1Color = colorTable[props[propMainLCDScrollbarCVInColordx]]

    jbox_display.draw_lines(scrollBarBorderPoints, cvIn1Color["dark"], "closed")
    jbox_display.draw_rect({left = box.left + scrollBarBoder, top = 3.5, right = box.right - scrollBarBoder, bottom = 12.5}, cvIn1Color["light"])
  else
    jbox_display.draw_lines(scrollBarBorderPoints, offColor, "closed")
  end
end

------------------
-- MainLCDScrollbarGesture
------------------

MainLCDScrollbarGesture = function(props, di, gsp)
  -- handle gesture only when connected and in pause mode...
  if props[propMainLCDScrollbarCVInStateIdx] == cvInStateConnected and
      props[propMainLCDScrollbarZoomFactorXIdx] ~= 0 and
      props[propMainLCDScrollbarScreenOnIdx] then
    local gesture_definition = {
      custom_data = {},
      handlers = {
        on_tap = "MainLCDScrollbarGestureHandlerTap",
        on_update = "MainLCDScrollbarGestureHandlerUpdate"
      }
    }
    return gesture_definition
  else
    return nil
  end
end

-- tap => start of gesture
MainLCDScrollbarGestureHandlerTap = function (props, di, gi, cd)
  local changes = {}
  if not props[propMainLCDScrollbarInputPausedIdx] then
    changes[propMainLCDScrollbarInputPausedIdx] = true
  end

  local box = computeZoomBox(props)

--  jbox.trace("box => min=" .. tostring(box.minCenter)
--      .. ";max=" .. tostring(box.maxCenter)
--      .. ";center=" .. tostring(box.center)
--      .. ";left=" .. tostring(box.left)
--      .. ";right=" .. tostring(box.right)
--      .. ";size=" .. tostring(box.size)
--  )

  local x = gi.current_point.x

  if x < box.left then
    box = moveZoomBox(box, -box.size)
  elseif x > box.right then
    box = moveZoomBox(box, box.size)
  else
    -- beginning of drag gesture... pause if necessary
    cd.startGesture = x
    cd.zoomBox = box
    if changes[propMainLCDScrollbarInputPausedIdx] then
      return { gesture_ui_name = jbox.ui_text("undo prop_input_paused"), property_changes = changes }
    else
      return changes
    end
  end

  -- after move, the cursor is in the handle => allow for drag
  if x >= box.left and x <= box.right then
    -- beginning of drag gesture... pause if necessary
    cd.startGesture = x
    cd.zoomBox = box
  end

  -- a bit hacky but works
  local newHistoryOffset = math.floor(computeNewHistoryOffset(box))
  if props[propMainLCDScrollbarInputHistoryOffsetRTIdx] ~= IGNORE_HISTORY_OFFSET_RT and
      props[propMainLCDScrollbarInputHistoryOffsetIdx] == newHistoryOffset then
    -- edge case when the value would not change...
    newHistoryOffset = clamp(newHistoryOffset + 1, 0, MAX_HISTORY_OFFSET)
  end

  -- zoombox has moved... need to recompute new offset
  changes[propMainLCDScrollbarInputHistoryOffsetIdx] = newHistoryOffset
  return { gesture_ui_name = jbox.ui_text("undo prop_input_history_offset"), property_changes = changes }
end

-- update => drag
MainLCDScrollbarGestureHandlerUpdate = function (props, di, gi, cd)
  local changes = {}

  if cd.zoomBox then
    local x = gi.current_point.x
    local box = moveZoomBox(cd.zoomBox, x - cd.startGesture)
    local inputHistoryOffset = computeNewHistoryOffset(box)
    changes[propMainLCDScrollbarInputHistoryOffsetIdx] = inputHistoryOffset
    return { gesture_ui_name = jbox.ui_text("undo prop_input_history_offset"), property_changes = changes }
  else
    return changes
  end
end

------------------
-- adjustCVValue
------------------
local IGNORE_CV_VALUE = 200001;

local cvDisplayScale1x = 0
local cvDisplayScale100x = 1
local cvDisplayScale127x = 2

-----------------------------------
-- adjustCVEqualAlgo (cvValue ~= 0)
-----------------------------------
function adjustCVEqualAlgo(cvScaleFactor, cvValue)

  cvScaleFactor = (cvScaleFactor * 2) + 1

  local isNegative = cvValue < 0

  local res = math.ceil(((math.abs(cvValue) * cvScaleFactor) - 1) / 2)
  if isNegative then
    res = -res
  end

  return res

end


local cvScaleFactors = { 100, 127 }

------------------
-- adjustCVValue
-------------------
function adjustCVValue(cvValue, cvValueInt, cvDisplayScale)

  if cvValue == IGNORE_CV_VALUE then
    return cvValue
  end

  local res

  if cvValueInt == 0 then
    res = (cvValue / 100000) - 1.0
  else
    res = (cvValueInt - 10001)
    if res < 0 then
      res = res - (cvValue / 100000)
    else
      res = res + (cvValue / 100000)
    end
  end

  -- handle 1x or simply 0 (since it is not affected by scale at all...)
  if cvDisplayScale == cvDisplayScale1x or equalsTo(res, 0) then
    return res
  end

  local factor = cvScaleFactors[cvDisplayScale]

  -- we clamp before computing since it can be > +/-1
  res = clamp(res, -1.0, 1.0)

  res = adjustCVEqualAlgo(factor, res)

  res = clamp(res, -factor, factor)

  return res
end

------------------
-- CVIn<x>ValueDraw
------------------
local propCVInValueIdx = 1
local propCVInValueIntIdx = 2
local propCVInDisplayScaleIdx = 3
local propCVInColorIdx = 4
local propCVInMinMaxResetIdx = 5

function CVValueDraw(handleMinMaxReset)
  return function(props, di, dr)

    local text = ""

    local cvInDisplayScale = props[propCVInDisplayScaleIdx]
    local cvInValue = adjustCVValue(props[propCVInValueIdx], props[propCVInValueIntIdx], cvInDisplayScale)

    if cvInValue ~= IGNORE_CV_VALUE then

      local format

      if cvInDisplayScale == cvDisplayScale1x then
        local v = math.abs(cvInValue)
        if v < 10.0 then
          format = "%.5f"
        elseif v < 100.0 then
          format = "%.4f"
        elseif v < 1000.0 then
          format = "%.3f"
        else
          format = "%.2f"
        end

      else
        format = "%03d"
      end

      if cvInValue < 0 then
        format = "-" .. format
      end

      text = string.format(format, math.abs(cvInValue))

      --    local text = string.format("%03d", 100 * cvInValue)
    else
      if cvInDisplayScale == cvDisplayScale1x then
        text = "-.----"
      else
        text = "- - -"
      end
    end

    if(handleMinMaxReset and props[propCVInMinMaxResetIdx]) then
      jbox_display.draw_rect({left = 5, top = 0, right = 55, bottom = 15}, colorTable[props[propCVInColorIdx]]["light"])
      jbox_display.draw_text({left = 0, top = 0, right = 60, bottom = 15},
        "center",
        text,
        textFont,
        blackColor
      )
    else
      jbox_display.draw_text({left = 0, top = 0, right = 60, bottom = 15},
        "center",
        text,
        textFont,
        colorTable[props[propCVInColorIdx]]["light"]
      )
    end
  end
end

CVIn1ValueDraw = CVValueDraw(false)
CVIn1MinValueDraw = CVValueDraw(true)
CVIn1MaxValueDraw = CVValueDraw(true)

local horizontalKeyboardVertices = {
  left = {
    {x = 0.0, y = 0.0 },
    {x = 0.0, y = 15.0 },
    {x = 6.0, y = 15.0 },
    {x = 6.0, y = 10.0 },
    {x = 4.0, y = 10.0 },
    {x = 4.0, y = 0.0 },
  },

  right = {
    {x = 2.0, y = 0.0 },
    {x = 2.0, y = 10.0 },
    {x = 0.0, y = 10.0 },
    {x = 0.0, y = 15.0 },
    {x = 6.0, y = 15.0 },
    {x = 6.0, y = 0.0 },
  },

  middle = {
    {x = 2.0, y = 0.0 },
    {x = 2.0, y = 10.0 },
    {x = 0.0, y = 10.0 },
    {x = 0.0, y = 15.0 },
    {x = 6.0, y = 15.0 },
    {x = 6.0, y = 10.0 },
    {x = 4.0, y = 10.0 },
    {x = 4.0, y = 0.0 },
  },

  sharp = {
    {x = 0.0, y = 0.0 },
    {x = 0.0, y = 8.0 },
    {x = 4.0, y = 8.0 },
    {x = 4.0, y = 0.0 },
  }
}

local verticalKeyboardVertices = {
  left = {
    {x = 20.0, y = 0.0 },
    {x = 31.0, y = 0.0 },
    {x = 31.0, y = 9.0 },
    {x = 0.0, y = 9.0 },
    {x = 0.0, y = 3.0 },
    {x = 20.0, y = 3.0 },
  },

  right = {
    {x = 0.0, y = 0.0 },
    {x = 31.0, y = 0.0 },
    {x = 31.0, y = 9.0 },
    {x = 20.0, y = 9.0 },
    {x = 20.0, y = 6.0 },
    {x = 0.0, y = 6.0 },
  },

  middle = {
    {x = 20.0, y = 0.0 },
    {x = 31.0, y = 0.0 },
    {x = 31.0, y = 9.0 },
    {x = 20.0, y = 9.0 },
    {x = 20.0, y = 6.0 },
    {x = 0.0, y = 6.0 },
    {x = 0.0, y = 3.0 },
    {x = 20.0, y = 3.0 },
  },

  sharp = {
    {x = 0.0, y = 0.0 },
    {x = 16.0, y = 0.0 },
    {x = 16.0, y = 5.0 },
    {x = 0.0, y = 5.0 },
  }
}


function offsetVertices(vertices, offset)
  local res = {}
  for k, v in pairs(vertices) do
    res[#res + 1] = { x = v.x + offset.x, y = v.y + offset.y }
  end
  return res
end

local horizontalNoteVertices = {
  ["C"] =  offsetVertices(horizontalKeyboardVertices["left"], {x = 0.0, y = 0.0} ),
  ["C#"] =  offsetVertices(horizontalKeyboardVertices["sharp"], {x = 5.0, y = 0.0} ),
  ["D"] =  offsetVertices(horizontalKeyboardVertices["middle"], {x = 8.0, y = 0.0} ),
  ["D#"] =  offsetVertices(horizontalKeyboardVertices["sharp"], {x = 13.0, y = 0.0} ),
  ["E"] =  offsetVertices(horizontalKeyboardVertices["right"], {x = 16.0, y = 0.0} ),
  ["F"] =  offsetVertices(horizontalKeyboardVertices["left"], {x = 24.0, y = 0.0} ),
  ["F#"] =  offsetVertices(horizontalKeyboardVertices["sharp"], {x = 29.0, y = 0.0} ),
  ["G"] =  offsetVertices(horizontalKeyboardVertices["middle"], {x = 32.0, y = 0.0} ),
  ["G#"] =  offsetVertices(horizontalKeyboardVertices["sharp"], {x = 37.0, y = 0.0} ),
  ["A"] =  offsetVertices(horizontalKeyboardVertices["middle"], {x = 40.0, y = 0.0} ),
  ["A#"] =  offsetVertices(horizontalKeyboardVertices["sharp"], {x = 45.0, y = 0.0} ),
  ["B"] =  offsetVertices(horizontalKeyboardVertices["right"], {x = 48.0, y = 0.0} ),
}

local verticalNoteVertices = {
  ["C"] =  offsetVertices(verticalKeyboardVertices["left"], {x = 0.0, y = 72.0} ),
  ["C#"] =  offsetVertices(verticalKeyboardVertices["sharp"], {x = 0.0, y = 68.0} ),
  ["D"] =  offsetVertices(verticalKeyboardVertices["middle"], {x = 0.0, y = 60.0} ),
  ["D#"] =  offsetVertices(verticalKeyboardVertices["sharp"], {x = 0.0, y = 56.0} ),
  ["E"] =  offsetVertices(verticalKeyboardVertices["right"], {x = 0.0, y = 48.0} ),
  ["F"] =  offsetVertices(verticalKeyboardVertices["left"], {x = 0.0, y = 36.0} ),
  ["F#"] =  offsetVertices(verticalKeyboardVertices["sharp"], {x = 0.0, y = 32.0} ),
  ["G"] =  offsetVertices(verticalKeyboardVertices["middle"], {x = 0.0, y = 24.0} ),
  ["G#"] =  offsetVertices(verticalKeyboardVertices["sharp"], {x = 0.0, y = 20.0} ),
  ["A"] =  offsetVertices(verticalKeyboardVertices["middle"], {x = 0.0, y = 12.0} ),
  ["A#"] =  offsetVertices(verticalKeyboardVertices["sharp"], {x = 0.0, y = 8.0} ),
  ["B"] =  offsetVertices(verticalKeyboardVertices["right"], {x = 0.0, y = 0.0} ),
}


-----------------------
-- CVIn1ValueAsKeyboardDraw
-----------------------
local propCVInValueAsNoteMidiOnIdx = 1
local propCVInValueAsNoteStateIdx = 2
local propCVInValueAsNoteIdx = 3
local propCVInValueAsNoteColorIdx = 4

CVIn1ValueAsKeyboardDraw = function(props, di, dr)

  local vertices

  if di.width > di.height then
    vertices = horizontalNoteVertices
  else
    vertices = verticalNoteVertices
  end

  if props[propCVInValueAsNoteMidiOnIdx] and props[propCVInValueAsNoteStateIdx] == cvInStateConnected then
    local midiNote = midiNotes[props[propCVInValueAsNoteIdx] + 1]

    local midiNote1Octave

    if midiNote:sub(2,2) == "#" then
      midiNote1Octave = midiNote:sub(1,2)
    else
      midiNote1Octave = midiNote:sub(1,1)
    end

    local darkColor = colorTable[props[propCVInValueAsNoteColorIdx]]["dark"]
    local lightColor = colorTable[props[propCVInValueAsNoteColorIdx]]["light"]

    for key, v in pairs(vertices) do
      local color = darkColor
      if key == midiNote1Octave then
        color = lightColor
      end
      jbox_display.draw_polygon(v, color)
    end
  else
    for key, v in pairs(vertices) do
      jbox_display.draw_polygon(v, offColor)
    end
  end
end

-----------------------
-- CVIn1ValueAsNoteDraw
-----------------------

CVIn1ValueAsNoteDraw = function(props, di, dr)

  local text
  local color

  if props[propCVInValueAsNoteMidiOnIdx] and props[propCVInValueAsNoteStateIdx] == cvInStateConnected then
    text = midiNotes[props[propCVInValueAsNoteIdx] + 1]
    color = colorTable[props[propCVInValueAsNoteColorIdx]]["light"]
  else
    text = "- - -"
    color = offColor
  end

  jbox_display.draw_text({left = 0, top = 0, right = di.width, bottom = di.height},
    "center",
    text,
    textFont,
    color
  )

end

-----------------------
-- CVIn1ValueAsGateDraw
-----------------------
local propCVInValueAsGateMidiOnIdx = 1
local propCVInValueAsGateIdx = 2
local propCVInValueIntAsGateIdx = 3
local propCVInValueAsGateColorIdx = 4

CVIn1ValueAsGateDraw = function(props, di, dr)

  local text
  local color

  local cvInValue = adjustCVValue(props[propCVInValueAsGateIdx], props[propCVInValueIntAsGateIdx], cvDisplayScale127x)

  if props[propCVInValueAsGateMidiOnIdx] and cvInValue ~= IGNORE_CV_VALUE then
    text = string.format("%03d", clamp(cvInValue, 0, 127))
    color = colorTable[props[propCVInValueAsGateColorIdx]]["light"]
  else
    text = "- - -"
    color = offColor
  end

  jbox_display.draw_text({left = 0, top = 0, right = di.width, bottom = di.height},
    "center",
    text,
    textFont,
    color
  )

end
