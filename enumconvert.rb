template = "\trb_define_const(mGimptest, \"%s\", INT2NUM(%s));"

strs = readlines
strs.collect! do|str|
	str = str.chomp[0..-1].delete(',').split.first
	sprintf(template, str[5..-1], str)
end

puts strs.join("\n")
puts