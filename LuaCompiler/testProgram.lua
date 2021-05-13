function testData(in1, in2) 
	-- test this data
	x = in1
	y = in2
	z = 0

	repeat 
		x = y - 4 * 2
		y = x / 2 - 3
		z = z + 1
	until z < 5

	if (x < y) then
		return 1
	elseif (x > y) then
		return 0
	else
		return nil
	end
end

result = testData(6, 4)
print("TEST")
print(testData(6, 4))
print(result)
print(testData(1, 0))
