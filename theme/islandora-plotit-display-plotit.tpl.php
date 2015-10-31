<?php
/**
 * @file
 * Islandora PlotIt Solr results template.
 *
 * Variables available:
 * - $json_callback_url: The Solr search URL for use in the preprocess.
 *
 * @see template_preprocess_islandora_plotit_display_ploitit()
 */
?>

<loading_overlay></loading_overlay>

<section id="sidebar">
    <header>
        <a href="/">Plot-It</a>
    </header>

    <section>
        <div id="filters">
            <text_filter params="label: 'Search', placeholder: 'eg. Rocky Mountains'"></text_filter>

            <date_filter params="label: 'Date Range'"></date_filter>


            <checklist_filter params="label: 'Collection', field: 'group'"></checklist_filter>
            <checklist_filter params="label: 'Event Type', field: 'eventType'"></checklist_filter>
        </div>

        <hr/>

        <filter_reset params="label: 'Reset All', filterGroupId: 'filters'"></filter_reset>
    </section>

    <section>
        <header>How to use search</header>
        <p>
            Search below by choosing a facet or typing keywords. To restart your search, click on "Reset all Filters"
            above the map.
        </p>

        <p>
            <a id="help" href="http://cwrc.ca/Plot-It_Documentation" target="_blank">Help...</a>
        </p>
    </section>

    <section>
        <header>
            Project team members
        </header>
        <p>
            Jeffery Antoniuk, Brittney Broder, Susan Brown, Michael Brundin, Lisa Dublin, Isobel Grundy, Kathryn
            Holland, Mihaela Ilovan, Hamman Samuel, <a href="http://tenjin.ca" target="_blank">Tenjin</a>, Kristina
            Vyskocil, and Willow White
        </p>
    </section>
</section>

<main>
    <expander params="expandedText: 'Hide Timeline', collapsedText: 'Show Timeline'">
        <timeline></timeline>
    </expander>

    <spotlight></spotlight>

    <style>
        grid .grid-Start,
        grid .grid-End {
            white-space: nowrap;
        }
    </style>
    <expander params="expandedText: 'Hide Map', collapsedText: 'Show Map'">
        <tab_pane>
            <map data-tab-label="Map View" params="center: '38.479394673276445, -115.361328125',
                                        zoom: 3,
                                        colorKey: 'group',
                                        colors: {BIBLIFO: '#f00', Multimedia: '#0f0', OrlandoEvents: '#00f', LGLC: '#ff0'},
                                        opacity: '0.5',
                                        markerWidth: 18,
                                        markerHeight: 18">
            </map>
            <grid data-tab-label="Grid View" params="columns: {'Label': 'label',
                                        'Collection': 'group',
                                        'Location': 'location',
                                        'Lat/Lng': 'latLng',
                                        'Start': 'startDate',
                                        'End': 'endDate'},
                              initialSortBy: ['group', 'label-za']">
            </grid>
        </tab_pane>
    </expander>
</main>


<!--
===============================================
               Component Templates
===============================================
-->
<div id="templates" style="display:none;">

    <!--
    ===============================================
                       Spotlight
    ===============================================
    -->
    <template id="spotlight-template">
        <section data-bind="if: selected() == null, visible: selected() == null">
            <p class="placeholder">
                Click an event in the timeline or map to see details
            </p>
        </section>
        <section data-bind="if: selected() != null, visible: selected() != null">
            <img data-bind="visible: selected().images, src: selected().images"/>
            <iframe allowfullscreen=true data-bind="visible: selected().videos, src: selected().videos">
            </iframe>
            <div>
                <header>
                    <span data-bind="if: selected().startDate">
                        <span data-bind="html: selected().startDate"></span>
                        <span data-bind="if: selected().endDate">
                            to
                            <span data-bind="text: selected().endDate"></span>
                        </span>
                    </span>
                    <span data-bind="if: selected().location">
                        <span data-bind="html: selected().location"></span>
                    </span>
                    <span data-bind="if: selected().link">
                        <a target="_blank" data-bind="html: 'link' , attr: { href: selected().link}"></a>
                    </span>
                </header>
                <section>
                    <header>
                        <span data-bind="html: selected().longLabel"></span>
                    </header>
                    <span data-bind="html: selected().description"></span>

                    <p data-bind="if: selected().urls">
                        <a target="_blank" data-bind="href: selected().urls">More...</a>
                    </p>

                    <p data-bind="if: selected().source">
                        <a target="_blank" data-bind="href: selected().source">(Source)</a>
                    </p>
                </section>
            </div>
        </section>
    </template>

    <!--
    ===============================================
                      Timeline
    ===============================================
    -->
    <template id="timeline-template">
        <section>
            <div>
                <span data-bind="text: unplottableCount"></span>
                of
                <span data-bind="text: CWRC.rawData().length"></span>
                lack time data
            </div>
        </section>
        <div data-bind="event: { mousedown: dragStart, touchstart: dragStart }">
            <section id="timeline-viewport" data-bind="event: {wheel: scrollHandler}">
                <div class="canvas" data-bind="foreach: timelineRows,
                                                           style: {
                                                                    width: canvasWidth,
                                                                    transform: zoomTransform,
                                                                    '-ms-transform': zoomTransform,
                                                                    '-webkit-transform': zoomTransform
                                                                  }">
                    <div class="row" data-bind="foreach: $data">
                        <a href="#" class="event" data-bind="css: { selected: $parents[1].isSelected($data) },
                                                             style: {
                                                                        left: $parents[1].getPinInfo($data).xPos,
                                                                        width: $parents[1].getPinInfo($data).width,
                                                                        color: $data.endDate ? 'red' : 'black'
                                                                    },
                                                             event: {mouseup: $parents[1].recordMouseUp, mousedown: $parents[1].recordMouseDown}">
                            <span data-bind="text: $data.startDate"></span>
                            <span data-bind="html: $data.label"></span>
                        </a>
                    </div>
                </div>
            </section>
            <!-- Ruler disabled until further production available
                        <section id="timeline-ruler">
                            <div data-bind="foreach: years,
                                            style: {
                                                        width: canvasWidth,
                                                        transform: rulerTransform
                                                    }">
                                <div data-bind="text: $data,
                                                style: {
                                                         left: $parent.labelPosition($data),
                                                         width: $parent.labelSize - $parent.lineThickness,
                                                        'border-left-width': $parent.lineThickness
                                                }"></div>
                            </div>
                        </section>-->
        </div>
    </template>


    <!--
    ===============================================
                         Grid
    ===============================================
    -->
    <template id="grid-template">
        <div id="gridSorting">
            Sort <span data-bind="visible: sortContexts().length > 0">by</span>
            <!-- Building a "widget" for editing each sorting context-->
            <div data-bind="foreach: sortContexts">
                <div>
                    <select data-bind="options: $parent.allFields, optionsText: 'name', optionsValue: 'name', optionsCaption:'Choose...', value: $data.name"></select>
                    <a href="#"
                       data-bind="click: function() { $data.reverse() }, text: $data.getFieldDirectionLabel()"></a>
                    <a href="#" data-bind="click: function() { $parent.removeSortBy($data) }">x</a>
                </div>
            </div>

            <a href="#" title="Add Sorting Rule" data-bind="click: addContext ">by...</a>
        </div>

        <table>
            <!-- explicit thead & tbody are important, otherwise the browser assumes incorrect things-->
            <thead>
            <tr>
                <th></th>
                <!-- ko foreach: Object.keys(columns) -->
                <th data-bind="text: $data">
                </th>
                <!-- /ko -->
            </tr>
            </thead>
            <tbody data-bind="foreach: {data: itemsOnCurrentPage, as: 'item'}">
            <tr data-bind="">
                <td>
                    <a href="#" data-bind="click: function(){ CWRC.selected(item)}">
                        Select
                    </a>
                </td>

                <!-- ko foreach: {data: Object.keys($parent.columns), as: 'columnLabel'} -->
                <td data-bind="html: item[$parents[1].columns[columnLabel]] || 'n/a', css: $parents[1].getColumnClass(columnLabel)">
                </td>
                <!-- /ko -->
                <td>
                    <a target="_blank" data-bind="attr: {href: item['link']}">
                        Link
                    </a>
                </td>
            </tr>
            </tbody>
        </table>

        <section data-bind="visible: maxPageIndex() > 1">
           <span data-bind="visible: isFarAway(1)">
                <a href="#"
                   data-bind="text: 1, click: function(){ setPage(1) }, attr: {selected: currentPageIndex() == 1}"></a>
                <span data-bind="visible: isFarAway(2)">
                    ...
                </span>
           </span>
           <span data-bind="foreach: pageNeighbourhood">
                <a href="#"
                   data-bind="text: $data, click: function(){ $parent.setPage($data) }, attr: {selected: $parent.currentPageIndex() == $data}"></a>
           </span>
           <span data-bind="visible: isFarAway(maxPageIndex())">
                <span data-bind="visible: isFarAway(maxPageIndex() - 1)">
                    ...
                </span>
                <a href="#"
                   data-bind="text: maxPageIndex, click: function(){ setPage(maxPageIndex()) }, attr: {selected: currentPageIndex() == maxPageIndex()}"></a>
           </span>
        </section>
    </template>


    <!--
    ===============================================
                        Map
    ===============================================
    -->
    <template id="map-template">
        <section>
            <div class="unplottable">
                <span data-bind="text: unplottableCount"></span>
                of
                <span data-bind="text: CWRC.rawData().length"></span>
                lack map data
            </div>
        </section>
        <div id="historicalMapControls" title="Click to toggle the historical map">
            <label>
                <input type="checkbox" data-bind="checked: showHistoricalMap">
                <span>Historical Map</span>
            </label>
            <label id="historicalOpacityControls" data-bind="visible: showHistoricalMap">
                <span>Opacity</span>
                <input id="historicalMapOpacity" type="range" min="0.0" max="1.0" step="0.05"
                       data-bind="value: historicalMapOpacity"/>
            </label>
        </div>
        <!-- identifying by ID does limit to one map per page, but that works for now -->
        <div id="map_canvas">
        </div>
        <section data-bind="visible: colorMap.hasMapping()">
            <header>Legend</header>
            <!-- ko foreach: colorMap.getLegendPairs() -->
            <div>
                <img data-bind="src: $data.icon"></span>
                <span data-bind="text: $data.name"></span>
            </div>
            <!-- /ko -->
        </section>
    </template>

    <!--
    ===============================================
                      Tab Pane
    ===============================================
    -->
    <template id="tab-pane-template">
        <header>
            <form data-bind="foreach: views">
                <label>
                    <input type="radio" name="tabset" data-bind="checked: $parent.currentView,
                                                             value: $index()"/>
                    <span data-bind="text: $parent.tabName($data, $index())"></span>
                </label>
            </form>
        </header>
        <div data-bind="foreach: views">
            <div data-bind="visible: $parent.isView($index()), template: {nodes: [$data]}">
            </div>
        </div>
    </template>

    <!--
    ===============================================
                     Error Status
    ===============================================
    -->
    <template id="status-template">
        <div data-bind="visible: hasMessages()">
            <section data-bind="visible: notices().length > 0">
                <ul>
                    <!-- ko foreach: notices() -->
                    <li>
                        <span data-bind="text: $data"></span>
                        <a href="#" data-bind="visible: $parent.dismissable,
                                                           click: function(){ $parent.notices.splice($index(), 1)}">X</a>
                    </li>
                    <!-- /ko -->
                </ul>
            </section>
            <section data-bind="visible: warnings().length > 0">
                <header>Warning</header>
                <p data-bind="text: warningFlavour(), visible: warningFlavour()"></p>
                <ul>
                    <!-- ko foreach: warnings() -->
                    <li>
                        <span data-bind="text: $data"></span>
                        <a href="#" data-bind="visible: $parent.dismissable,
                                                           click: function(){ $parent.warnings.splice($index(), 1)}">X</a>
                    </li>
                    <!-- /ko -->
                </ul>
            </section>
            <section data-bind="visible: errors().length > 0">
                <ul>
                    <!-- ko foreach: errors() -->
                    <li>
                        <span data-bind="text: $data"></span>
                        <a href="#" data-bind="visible: $parent.dismissable,
                                                           click: function(){ $parent.errors.splice($index(), 1)}">X</a>
                    </li>
                    <!-- /ko -->
                </ul>
            </section>
        </div>
    </template>

</div>
