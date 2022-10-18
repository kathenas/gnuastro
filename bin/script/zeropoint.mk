# Creat final PSF for all tiles and all filters.
#
# Original authors:
# Copyright (C) 2019-2022 Samane Raji <samaneraji@gmail.com>
#
# Contributers:
# Copyright (C) 2019-2022 Mohammad Akhlaghi <mohammad@akhlaghi.org>
# Copyright (C) 2019-2022 Zahra sharbaf <ahra.sharbaf2@gmail.com>
# Copyright (C) 2022 Sepideh Eskandarlou <sepideh.eskandarlou@gmail.com>
#
# This Makefile is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This Makefile is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this Makefile.  If not, see <http://www.gnu.org/licenses/>.
# Set input & Final target






# Final target.
all: final

# Make all the commands in the recipe in one shell.
.ONESHELL:

# Second expantion.
.SECONDEXPANSION:

# Stop the recipe's shell if the command fails
.SHELLFLAGS = -ec

# Include configure files.
include zeropoint.conf




# Build the main directory for saving the outputs.
$(tmpdir):; mkdir $@





# Catalog of stars
# ----------------
#
# Use Gaia catalog and only keep the objects with good parallax (to
# confirm that they are stars).
stars=$(tmpdir)/gaia.fits
$(stars): $(indir)/$(input) | $(tmpdir)

#	Download from Gaia.
	raw=$(subst .fits,-raw.fits,$@)
	astquery gaia --dataset=dr3 \
	         --overlapwith=$(indir)/$(input) \
	         -csource_id -cra -cdec -cparallax \
	         -cphot_g_mean_mag -cparallax_error \
	         -cpmra -cpmdec --output=$$raw

#	Only keep stars (with good parallax).
	asttable $$raw -cra,dec \
	         -cphot_g_mean_mag --colinfoinstdout \
	         -c'arith parallax parallax abs \
	                  parallax_error 3 x lt nan where ' \
	         --colmetadata=4,PARALLAX,int32,"Stars with good parallax." \
	         --noblankend=PARALLAX \
	         | asttable -cra,dec -cphot_g_mean_mag \
	                    --output=$@

#	Clean up.
	rm $$raw





# Apertures photometry
# --------------------
#
# To generate the apertures catalog we’ll use Gnuastro’s MakeProfiles
# We’ll first read the positions from the Gaia catalog, then use AWK
# to set the other parameters of each profile to be a fixed circle of
# radius 5.1 pixels. To calculate the pixel size I have to use the big
# origin image before cropping
inputzp=0
ifeq ($(reftype),img)
gencat=$(foreach i, $(refnumber), ref$(i))
else
gencat=
endif
aperture=$(foreach i,input $(gencat), \
          $(foreach a,$(aper-arcsec), \
           $(tmpdir)/$(i)-$(a)-cat.fits))
$(aperture): $(tmpdir)/%-cat.fits: $(stars)

#	Extract the names.
	img=$($(word 1, $(subst -, ,$*)))
	zp=$($(word 1, $(subst -, ,$*))zp)
	aperarcsec=$(word 2, $(subst -, ,$*))
	hdu=$($(word 1, $(subst -, ,$*))hdu)

#	Convert the aperture size (arcsec) to pixels.
	aperpix=$$(astfits $$img --hdu=$$hdu --pixelscale --quiet \
	                   | awk -v s=$$aperarcsec \
	                         '{print s/($$1 * 3600)}')

#	Make an aperture catalog by using aperture size in pixels
	aperwcscat=$(subst .fits,-aperwcscat.txt,$@)
	asttable $(stars) -cra,dec \
	         | awk -v r=$$aperpix \
	               '{ print NR, $$1, $$2, 5, r, 0, 0, 1, NR, 1 }' \
	         > $$aperwcscat

#	Make an image of apertures.
	aperimg=$(subst .fits,-aper.fits,$@)
	astmkprof $$aperwcscat --background=$$img --backhdu=$$hdu \
	          --clearcanvas --replace --type=int32 --mforflatpix \
	          --mode=wcs --output=$$aperimg --quiet

#	Build a catalog of this aperture image.
	astmkcatalog $$aperimg -h1 --output=$@ --zeropoint=$$zp \
	             --inbetweenints --valuesfile=$$img \
	             --valueshdu=$$hdu --ids --ra --dec --magnitude

#	Clean up.
	rm $$aperwcscat $$aperimg




# Prepare the catalog for comparing in different apperture.
cataper=$(foreach a,$(aper-arcsec), \
          $(tmpdir)/ref1-$(a)-cat.fits)
$(cataper): $(tmpdir)/ref1-%-cat.fits: $(tmpdir)/input-%-cat.fits

	asttable $(ref1) -cRA,DEC -cMAGNITUDE \
	         | cat -n \
	         | asttable --output=$@ \
	               --colmetadata=1,OBJ_ID,int32,"Id of object." \
	               --colmetadata=2,RA,float64,"Right Assencion." \
	               --colmetadata=3,DEC,float64,"Declination." \
	               --colmetadata=4,MAGNITUDE,float32,"Magnitude."








# Calculate magnitude differences
# -------------------------------
#
# Match reference catalog with input catalog and put reference
# magnitude in the catalog. Then subtract the reference mag from input
# mag. Finally, the final target has two columns of reference mag and
# subtracted mag.
allrefs=$(foreach i, $(refnumber), ref$(i))
magdiff=$(foreach r,$(allrefs), \
         $(foreach a,$(aper-arcsec), \
          $(tmpdir)/$(r)-$(a)-magdiff.fits))
$(magdiff): $(tmpdir)/%-magdiff.fits: $(tmpdir)/%-cat.fits \
            $(tmpdir)/input-$$(word 2,$$(subst -, ,%))-cat.fits

#	Find the matching objects in both catalogs. Note that the
#	labels are the same so we don't need to use RA,Dec
	ref=$(tmpdir)/$*-cat.fits
	match=$(subst .fits,-match.fits,$@)
	input=$(tmpdir)/input-$(word 2,$(subst -, ,$*))-cat.fits
	astmatch $$ref --hdu=1 $$input --hdu2=1 \
	         --ccol1=OBJ_ID --ccol2=OBJ_ID --aperture=0.2 \
	         --outcols=aMAGNITUDE,bMAGNITUDE \
	         --output=$$match

#	Subtract the refrence catalog mag from input catalog's mag.
	asttable $$match -c1 -c'arith $$1 $$2 -' \
	         --colmetadat=1,MAG-REF,f32,"Magnitude of reference." \
                 --colmetadat=2,MAG-DIFF,f32,"Magnitude diff with input." \
	         --noblankend=1,2 --output=$@

#	Clean up.
	rm $$match






# Zeropoint for each aperture
# ---------------------------
#
# Finding the Zeropoint. Calculate Zeropoint number in seperated file
# and calculate the root mean square of Zeropoint.
aperzeropoint=$(foreach a,$(aper-arcsec),$(tmpdir)/zeropoint-$(a).txt)
$(aperzeropoint): $(tmpdir)/zeropoint-%.txt: \
                  $$(foreach r,$(allrefs),$(tmpdir)/$$(r)-%-magdiff.fits)

#	Merge all the magdiffs from all the references.
	opts=""
	if [ "$(refnumber)" != 1 ]; then
	  for r in $$(echo $(refnumber) | sed -e's|1||'); do
	    opts="$$opts --catrowfile=$(tmpdir)/ref$$r-$*-magdiff.fits"
	  done
	fi

#	Merge all the rows from all the reference images into one.
	merged=$(subst .txt,-merged.fits,$@)
	asttable $(tmpdir)/ref1-$*-magdiff.fits $$opts -o$$merged

#	If the user requested a certain magnitude range, use it.
	rangeopt=""
	if [ x"$(magrange)" != x ]; then
	  rangeopt="--range=MAG-REF,$(magrange)"
	fi

#	Find the statistic zeropoint and write it into the target.
	zpstd=$$(asttable $$merged $$rangeopt -cMAG-DIFF \
	                  | aststatistics --sigclip-median \
	                                  --sigclip-std -q)
	echo "$* $$zpstd" > $@





# Most accurate zeropoint
# -----------------------
#
# Using the standard deviation of the zeropoints for each aperture,
# select the one with the least scatter.
zeropoint=$(tmpdir)/zeropoint.txt
$(zeropoint): $(aperzeropoint)
	zp=$(subst .txt,-tmp.txt,$@)
	echo "# Column 1: APERTURE  [arcsec,f32,]" > $$zp
	echo "# Column 2: ZEROPOINT [mag,f32,]"  >> $$zp
	echo "# Column 3: ZPSTD     [mag,f32,]"  >> $$zp
	for a in $(aper-arcsec); do
	  cat $(tmpdir)/zeropoint-$$a.txt        >> $$zp
	done
	asttable $$zp --sort=ZPSTD --head=1 \
	         --column=APERTURE,ZEROPOINT >$@





# Final target.
final: $(zeropoint)